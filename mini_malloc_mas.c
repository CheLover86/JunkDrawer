#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

typedef struct metadata {
	size_t size;
	bool isFree;
} metadata_t;

#define HEADER_SIZE	sizeof(metadata_t)
#define OVERHEAD	(2 * HEADER_SIZE)
#define ALIGNMENT	16
#define MIN_BLOCK_SIZE	(OVERHEAD + ALIGNMENT)
#define ARENA_SIZE	(1024 * 1024)

_Static_assert(sizeof(metadata_t) % ALIGNMENT == 0,
	       "header size must keep payloads 16-byte aligned");

static metadata_t *heapStart = NULL;
static char *heapEnd = NULL;

/* ---------- helpers ---------- */

static metadata_t *footer_of(metadata_t *h) {
	return (metadata_t *)((char *)h + h->size - HEADER_SIZE);
}

static void set_block(metadata_t *h, size_t size, bool isFree) {
	h->size = size;
	h->isFree = isFree;

	metadata_t *f = footer_of(h);
	f->size = size;
	f->isFree = isFree;
}

static void heap_check(void) {
	if (heapStart == NULL) {
		return;
	}

	metadata_t *cur = heapStart;
	bool prevFree = false;

	while ((char *)cur < heapEnd) {
		assert(cur->size % ALIGNMENT == 0);
		assert(cur->size >= MIN_BLOCK_SIZE);
		assert((char *)cur + cur->size <= heapEnd);
		assert(((uintptr_t)cur + HEADER_SIZE) % ALIGNMENT == 0);

		metadata_t *f = footer_of(cur);
		assert(f->size == cur->size);
		assert(f->isFree == cur->isFree);

		assert(!(prevFree && cur->isFree));
		prevFree = cur->isFree;

		cur = (metadata_t *)((char *)cur + cur->size);
	}
	assert((char *)cur == heapEnd);
}

#ifdef HEAP_DEBUG
#define CHECK_HEAP() heap_check()
#else
#define CHECK_HEAP() ((void)0)
#endif

static void heap_dump(void) {
	for (metadata_t *cur = heapStart; (char *)cur < heapEnd;
	     cur = (metadata_t *)((char *)cur + cur->size)) {
		printf("  [%6zu] %-9s size=%zu\n", (size_t)((char *)cur - (char *)heapStart),
		       cur->isFree ? "free" : "allocated", cur->size);
	}
}

/* ---------- allocator ---------- */

static bool heap_init(void) {
	void *raw = sbrk(ARENA_SIZE + ALIGNMENT);
	if (raw == (void *)-1) {
		return false;
	}

	uintptr_t base = ((uintptr_t)raw + ALIGNMENT - 1) & ~(uintptr_t)(ALIGNMENT - 1);
	heapStart = (metadata_t *)base;
	heapEnd = (char *)heapStart + ARENA_SIZE;

	set_block(heapStart, ARENA_SIZE, true);
	return true;
}

void *my_malloc(size_t size) {
	if (size == 0 || size > ARENA_SIZE) {
		return NULL;
	}

	if (heapStart == NULL && !heap_init()) {
		return NULL;
	}

	size_t targetSize = (size + ALIGNMENT - 1) & ~(size_t)(ALIGNMENT - 1);
	size_t needed = targetSize + OVERHEAD;

	metadata_t *current = heapStart;

	while ((char *)current < heapEnd) {
		if (current->isFree && current->size >= needed) {
			size_t remainder = current->size - needed;

			if (remainder >= MIN_BLOCK_SIZE) {
				set_block(current, needed, false);

				metadata_t *next = (metadata_t *)((char *)current + needed);
				set_block(next, remainder, true);
			} else {
				set_block(current, current->size, false);
			}

			CHECK_HEAP();
			return (char *)current + HEADER_SIZE;
		}

		current = (metadata_t *)((char *)current + current->size);
	}

	return NULL;
}

void my_free(void *ptr) {
	if (ptr == NULL) {
		return;
	}

	metadata_t *header = (metadata_t *)((char *)ptr - HEADER_SIZE);

	if (heapStart == NULL || (char *)header < (char *)heapStart ||
	    (char *)header >= heapEnd || header->isFree) {
		return;
	}

	size_t size = header->size;

	metadata_t *next = (metadata_t *)((char *)header + size);
	if ((char *)next < heapEnd && next->isFree) {
		size += next->size;
	}

	if (header > heapStart) {
		metadata_t *prevFooter = (metadata_t *)((char *)header - HEADER_SIZE);
		if (prevFooter->isFree) {
			header = (metadata_t *)((char *)header - prevFooter->size);
			size += prevFooter->size;
		}
	}

	set_block(header, size, true);

	CHECK_HEAP();
}

/* ---------- tests ---------- */

static void test_basic(void) {
	printf("Test 1: basic alloc / free / reuse\n");

	char *str1 = my_malloc(20);
	assert(str1 != NULL);
	strcpy(str1, "Sup guys?");
	printf("  str1 = %s\n", str1);

	int *arr = my_malloc(sizeof(int) * 5);
	assert(arr != NULL);
	for (int i = 0; i < 5; i++) {
		arr[i] = (i + 1) * 10;
	}
	printf("  arr[2] = %d\n", arr[2]);

	my_free(str1);

	const char msg[] = "Some free space...";
	char *str2 = my_malloc(sizeof(msg));
	assert(str2 != NULL);
	strcpy(str2, msg);
	printf("  str2 = %s (reused str1's block: %s)\n", str2, str2 == str1 ? "yes" : "no");

	
	for (int i = 0; i < 5; i++) {
		assert(arr[i] == (i + 1) * 10);
	}

	my_free(str2);
	my_free(arr);
	assert(heapStart->isFree && heapStart->size == ARENA_SIZE);
}

static void test_stale_footer(void) {
	printf("Test 2: the stale-footer scenario that broke the old version\n");

	char *a = my_malloc(20);
	char *b = my_malloc(20);
	char *c = my_malloc(20);
	my_free(a);
	char *d = my_malloc(15);
	assert(d == a);
	my_free(b);

	metadata_t *dh = (metadata_t *)(d - HEADER_SIZE);
	assert(!dh->isFree && dh->size == 64);

	my_free(c);
	my_free(d);
	assert(heapStart->isFree && heapStart->size == ARENA_SIZE);
}

static void test_coalescing(void) {
	printf("Test 3: coalescing in both directions\n");

	char *a = my_malloc(100);
	char *b = my_malloc(100);
	char *c = my_malloc(100);
	char *d = my_malloc(100);

	my_free(b);
	my_free(d);
	my_free(c);
	heap_dump();

	char *big = my_malloc(300);
	assert(big == b);

	my_free(a);
	my_free(big);
	assert(heapStart->isFree && heapStart->size == ARENA_SIZE);
}

static void test_stress(void) {
	printf("Test 4: random alloc/free with overlap detection\n");

	enum { SLOTS = 64, ROUNDS = 20000 };
	unsigned char *ptr[SLOTS] = {0};
	size_t len[SLOTS] = {0};

	srand(42);
	for (int r = 0; r < ROUNDS; r++) {
		int i = rand() % SLOTS;

		if (ptr[i] != NULL) {
			for (size_t k = 0; k < len[i]; k++) {
				assert(ptr[i][k] == (unsigned char)i);
			}
			my_free(ptr[i]);
			ptr[i] = NULL;
		} else {
			len[i] = 1 + (size_t)(rand() % 500);
			ptr[i] = my_malloc(len[i]);
			assert(ptr[i] != NULL);
			assert((uintptr_t)ptr[i] % ALIGNMENT == 0);
			memset(ptr[i], i, len[i]);
		}
		heap_check();
	}

	for (int i = 0; i < SLOTS; i++) {
		my_free(ptr[i]);
	}
	assert(heapStart->isFree && heapStart->size == ARENA_SIZE);
}

int main(void) {
	test_basic();
	test_stale_footer();
	test_coalescing();
	test_stress();

	printf("All tests completed successfully!\n");
	return 0;
}
