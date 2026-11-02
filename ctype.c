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

int main(void) {
    puts("Testing mini_ctype...\n");

    // my_isdigit()
    assert(my_isdigit('0'));
    assert(my_isdigit('9'));
    assert(!my_isdigit('a'));
    assert(!my_isdigit('/'));
    puts("[PASS] my_isdigit");

    // my_isalpha()
    assert(my_isalpha('A'));
    assert(my_isalpha('Z'));
    assert(my_isalpha('a'));
    assert(my_isalpha('z'));
    assert(!my_isalpha('0'));
    assert(!my_isalpha('@'));
    puts("[PASS] my_isalpha");

    // my_isalnum()
    assert(my_isalnum('A'));
    assert(my_isalnum('z'));
    assert(my_isalnum('5'));
    assert(!my_isalnum('!'));
    assert(!my_isalnum(' '));
    puts("[PASS] my_isalnum");

    // my_isspace()
    assert(my_isspace(' '));
    assert(my_isspace('\t'));
    assert(my_isspace('\n'));
    assert(my_isspace('\v'));
    assert(my_isspace('\r'));
    assert(my_isspace('\f'));
    assert(!my_isspace('A'));
    puts("[PASS] my_isspace");

    // my_isupper()
    assert(my_isupper('A'));
    assert(my_isupper('Z'));
    assert(!my_isupper('a'));
    assert(!my_isupper('0'));
    puts("[PASS] my_isupper");

    // my_islower()
    assert(my_islower('a'));
    assert(my_islower('z'));
    assert(!my_islower('A'));
    assert(!my_islower('0'));
    puts("[PASS] my_islower");

    // my_toupper()
    assert(my_toupper('a') == 'A');
    assert(my_toupper('z') == 'Z');
    assert(my_toupper('A') == 'A');
    assert(my_toupper('7') == '7');
    assert(my_toupper('!') == '!');
    puts("[PASS] my_toupper");

    // my_tolower()
    assert(my_tolower('A') == 'a');
    assert(my_tolower('Z') == 'z');
    assert(my_tolower('a') == 'a');
    assert(my_tolower('7') == '7');
    assert(my_tolower('!') == '!');
    puts("[PASS] my_tolower");

    puts("\nAll tests passed!");

    return 0;
}
