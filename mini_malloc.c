#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

typedef struct metadata {
	size_t size;
	bool isFree;	
} metadata_t;

#define HEADER_SIZE sizeof(metadata_t)
#define MIN_BLOCK_SIZE 32

metadata_t *heapStart = NULL;

void* my_malloc(size_t size) {
	if (size == 0) {
		return NULL;
	}

	size_t targetSize = (size + 7) & ~7;

	metadata_t *current = heapStart;
	
	while (current != NULL && (void *)current < sbrk(0)) {
	printf("Traversing block at %p, size: %zu, isFree: %d\n", (void *)current, current->size, current->isFree);
	
	if (current->size == 0) {
		break;
	}

		if (current->isFree == true && current->size >= (targetSize + (2 * HEADER_SIZE))) {
			size_t remainderSize = current->size - (targetSize + (2 * HEADER_SIZE));
		       if (remainderSize >= MIN_BLOCK_SIZE) {
		       		metadata_t *nextBlock = (metadata_t *)((char *)current + targetSize + (2 * HEADER_SIZE));
				nextBlock->size = remainderSize;
				nextBlock->isFree = true;

				metadata_t *nextFooter = (metadata_t *)((char *)nextBlock + nextBlock->size - HEADER_SIZE);
				nextFooter->size = nextBlock->size;
				nextFooter->isFree = nextBlock->isFree;

				current->size = targetSize + (2 *HEADER_SIZE);	
		       }
		       current->isFree = false;

		       return (void *)((char *)current + HEADER_SIZE);
		}
		metadata_t *currentFooter = (metadata_t *)((char *)current + current->size + HEADER_SIZE);
		current = (metadata_t *)((char *)current + current->size);
	}
	size_t totalBytesNeeded = targetSize + (2 * HEADER_SIZE);
	void *rawMemory = sbrk(totalBytesNeeded);

	if (rawMemory == (void *)-1) {
		return NULL;
	}

	metadata_t *newBlock = (metadata_t *)rawMemory;
	newBlock->size = totalBytesNeeded;
	newBlock->isFree = false;
	
	metadata_t *newFooter = (metadata_t *)((char *)newBlock + newBlock->size - HEADER_SIZE);
	newFooter->size = newBlock->size;
	newFooter->isFree = newBlock->isFree;

	if (heapStart == NULL) {
		heapStart = newBlock;
	}

	return (void *)((char *)newBlock + HEADER_SIZE);	
}

void my_free(void *ptr) {
	if (ptr == NULL) {
		return;
	}

	metadata_t *header = (metadata_t *)((char *)ptr - HEADER_SIZE);
	header->isFree = true;

	metadata_t *footer = (metadata_t *)((char *)header + header->size - HEADER_SIZE);
	footer->isFree = true;

	if ((void *)header > (void *)heapStart) {
		metadata_t *previousFooter = (metadata_t *)((char *)header - HEADER_SIZE);

		if (previousFooter->isFree == true) {
			metadata_t *previousHeader = (metadata_t *)((char *)header - previousFooter->size);
			previousHeader->size += header->size;
			header = previousHeader;

			footer = (metadata_t *)((char *)header + header->size - HEADER_SIZE);
			footer->size = header->size;
		}
	}
	
	metadata_t *nextBlock = (metadata_t *)((char *)header + header->size);

	if ((void *)nextBlock < sbrk(0) && nextBlock->isFree == true) {
		header->size += nextBlock->size; 

		footer = (metadata_t *)((char *)header + header->size - HEADER_SIZE);
		footer->size = header->size;
		footer->isFree = true;
	}	
}

int main(void) {
	char *str1 = (char *)my_malloc(20);

	if (str1 == NULL) {
		printf("str allocation failed.\n");
	}

	strcpy(str1, "Sup guys?");
	printf("Test 1: %s\n", str1);

	int *arr = (int *)my_malloc(sizeof(int) * 5);
	if (arr == NULL) {
		printf("arr allocation failed!");
	}

	for (int i = 0; i < 5; i++) {
		arr[i] = (i + 1) * 10;
	}

	printf("Arr[2] %d\n", arr[2]);

	printf("Freeing str1.... \n");
	my_free(str1);

	char *str2 = (char *)my_malloc(15);
	strcpy(str2, "Some free space...");

	printf("str2 -> %s\n", str2);

	printf("All tests completeled successfully!\n");

	return 0;
}
