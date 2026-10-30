#include <stdbool.h>
#include "ministring.h"

int my_memcmp(const void *s1, const void *s2, size_t n) {
	const unsigned char *tmp_s1 = s1;
	const unsigned char *tmp_s2 = s2;
	size_t counter = 0;

	while (counter < n) {
		if (*tmp_s1 != *tmp_s2) {
			return *tmp_s1 - *tmp_s2;
		}

		tmp_s1++;
		tmp_s2++;
		counter++;
	}

	return 0;
}

void *my_memcpy(void *restrict dst, const void *restrict src, size_t n) {
	const unsigned char* tmp_src = src;
	unsigned char* tmp_dst = dst;

	for (size_t i = 0; i < n; i++) {
		tmp_dst[i] = tmp_src[i];
	}

	return dst;
}

char *my_strcat(char *dest, const char *src) {
	char *tmp_dest = dest;
	const char *tmp_src = src;

	while (*tmp_dest != '\0') {
		tmp_dest++;
	}

	while (*tmp_src != '\0') {
		*tmp_dest = *tmp_src;
		tmp_src++;
		tmp_dest++;
	}

	*tmp_dest = '\0';

	return dest;
}

int my_strcmp(const char *s1, const char *s2) {
	const char *tmp_s1 = s1;
	const char *tmp_s2 = s2;

	while (*tmp_s1 != '\0' && *tmp_s2 != '\0') {
		if (*tmp_s1 != *tmp_s2) {
			return (unsigned char)*tmp_s1 - (unsigned char)*tmp_s2;
		}
	
		tmp_s1++;
		tmp_s2++;
	}

	return (unsigned char)*tmp_s1 - (unsigned char)*tmp_s2;
}

char *my_strcpy(char *dst, const char *src) {
	char *tmp_dst = dst;
	const char *ptr = src;
	bool isRunning = true;

	while (isRunning) {
		*tmp_dst = *ptr;

		if (*ptr == '\0') {
			isRunning = false;	
		}
		
		ptr++;
		tmp_dst++;
	}	

	return dst;

}

size_t my_strlen(const char *s) {
	const char *tmp = s;
	size_t count = 0;

	while(*tmp != '\0') {
		count++;
		tmp++;
	}

	return count;
}

char *my_strncpy(char *dst, const char *src, size_t len) {
	char *tmp_dst = dst;
	const char *ptr = src;
	size_t i;

	for (i = 0; i < len && ptr[i] != '\0'; i++) {
		tmp_dst[i] = ptr[i];
	}


	for(; i < len; i++) {
		tmp_dst[i] = '\0';
	}

	return dst;
}

char *my_strstr(const char *haystack, const char *needle) {
	const char *tmp_hay = haystack;

	if (*needle == '\0') {
		return (char *)haystack;
	}

	while (*tmp_hay != '\0') {
		const char *check_hay = tmp_hay;
		const char *check_ned = needle;
		
		while (*check_hay == *check_ned && *check_hay != '\0') {
				check_ned++;
				check_hay++;
				
		}	

		if (*check_ned == '\0') {
			return (char *)tmp_hay;
		}

		tmp_hay++;
	}

	return NULL;
}

void *my_memset(void *b, int c, size_t len) {
	unsigned char *tmp_b = b;

	for (size_t i = 0; i < len; i++) {
		tmp_b[i] = (unsigned char)c;
	}

	return b;
}
