#include <stdio.h>
#include <assert.h>

int my_isdigit(int c) {
	return c >= '0' && c <= '9';
}

int my_isalpha(int c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int my_isalnum(int c) {
	return my_isalpha(c) || my_isdigit(c);
}

int my_isspace(int c) {
	return c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\v' || c == '\f';
}

int my_isupper(int c) {
	return c >= 'A' && c <= 'Z';
}

int my_islower(int c) {
	return c >= 'a' && c <= 'z';
}

int my_toupper(int c) {
	if (my_islower(c)) {
		return c - 32;
	}

	return c;
}

int my_tolower(int c) {
	if (my_isupper(c)) {
		return c + 32;
	}

	return c;
}
