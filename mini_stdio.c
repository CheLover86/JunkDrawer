#include <unistd.h>
#include <stdio.h>

int my_putchar(int c) {
	unsigned char ch = (unsigned char)c;

	ssize_t result = write(STDOUT_FILENO, &ch, 1);

	if (result == -1) return EOF; 
	
	else if (result == 0) return EOF;

	return (int)(unsigned char)c;
}

int my_puts(const char *s) {
	const char *ptr = s;

	if (ptr == NULL) return EOF;

	while (*ptr != '\0') {
		int result = my_putchar((unsigned char)*ptr);
		if (result == EOF) return EOF;
		ptr++;
	}

	if (my_putchar('\n') == EOF) return EOF;

	return 0;
}

int my_getchar(void) {
	unsigned char a;

	ssize_t result = read(STDIN_FILENO, &a, 1);

	if (result == -1) return EOF;

	else if (result == 0) return EOF;

	return (int)(unsigned char)a;
}

char *my_fgets(char *str, int size) {
	if (str == NULL || size <= 0) return NULL;
	
	char *tmp = str;
	int result;
	int count = 0;

	while (count < size - 1) {
		result = my_getchar();

		if (result == EOF) {
			if (count == 0) return NULL;
			break;
		}
		
		*str = (char)result;

		str++;
		count++;

		if (result == '\n') break;
		
	}
	*str = '\0';

	return tmp;
}

