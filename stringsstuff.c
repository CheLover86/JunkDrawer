#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>

#define SIZE 100

size_t my_strlen(const char *s);
char *my_strup(const char *s);
char *my_strcpy(char *dest, const char *src);
int my_printf(const char *format, ...);
void print_to_screen(char c);

int main(void) {

	my_printf("I DID IT!\n");

	return 0;

}


size_t my_strlen(const char *s) {
	
	size_t count = 0;
	
	while(*s != '\0') {
		s++;
		count++;
	}
	return count;
}

char *my_strup(const char *s) {
	size_t total_space = my_strlen(s) + 1;

	char *new_space = (char *)malloc(total_space * sizeof(char));

	if (new_space == NULL) {
		return NULL;
	}

	for (size_t i = 0; i < total_space; i++) {
		new_space[i] = s[i];
	}

	return new_space;
}

char *my_strcpy(char *dest, const char *src) {
	char *original_dest = dest;

	while (*src != '\0') {
		*dest = *src;
		src++;
		dest++;
	}
	*dest = '\0';
	
	return original_dest;
}

void print_to_screen(char c) {
	write(1, &c, 1);
}

int my_printf(const char *format, ...) {
	const char *ptr = format;
	int count = 0;
	va_list args;
	va_start(args, format);
	
	while (*ptr != '\0') {
		if (*ptr != '%') {
			print_to_screen(*ptr);
			ptr++;
			count++;
		}
		else {
			ptr++;
			switch (*ptr) {
				case 'c':
					int ch = va_arg(args, int);
					print_to_screen(ch);
					count++;
					ptr++;
					break;
				case 's':
					char *str = va_arg(args, char *);
					while (*str != '\0') {
						print_to_screen(*str);
						str++;
						count++;
					}
					ptr++;
					break;
				case 'i':
				case 'd':
					int num = va_arg(args, int);
					if (num == 0) {
						print_to_screen('0');
						count++;
						ptr++;
						break;
					}
					else {
						if (num < 0) {
							print_to_screen('-');
							count++;
							num *= -1;
						}

						char buffer[12];
						int i = 0;

						while (num > 0) {
							buffer[i] = (num % 10) + '0';
							i++;
							num /= 10;
						}

						while (i > 0) {
							i--;
							print_to_screen(buffer[i]);
							count++;
						}
					}
					ptr++;
					break;
				case '%':
					print_to_screen(*ptr);
					count++;
					ptr++;
					break;
				default:
					ptr;
					break;
			}
		}
	}
	va_end(args);
	return count;
}
