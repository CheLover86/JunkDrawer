#ifndef MINISTRING_H
#define MINISTRING_H

#include <stddef.h>

char *my_strncpy(char *dst, const char *src, size_t len);
size_t my_strlen(const char *s);
char *my_strcpy(char *dst, const char *src);
void *my_memcpy(void *restrict dst, const void *restrict src, size_t n);
char *my_strcat(char *dest, const char *src);
int my_strcmp(const char *s1, const char *s2);
int my_memcmp(const void *s1, const void *s2, size_t n);
char *my_strstr(const char *haystack, const char *needle);
void *my_memset(void *b, int c, size_t len);

#endif
