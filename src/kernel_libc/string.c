#include "kernel_libc/string.h"

void* memset(void *ptr, uint8_t value, uint32_t size) {
	for (uint32_t i = 0; i < size; i++) {
		((uint8_t*) ptr)[i] = value;
	}
	return ptr;
}

void* memcpy(void *destination, const void *source, uint32_t num) {
	for (int index = 0; index < num; index++) {
		((uint8_t*) destination)[index] = ((uint8_t*) source)[index];
	}

	return destination;
}

int strlen(const char *s) {
	int index = 0;
	while (s[index] != 0) {
		index++;
	}
	return index;
}

int strcmp(const char *s1, const char *s2) {
	int differences = 0;
	int index1 = 0;
	int index2 = 0;
	int len1 = strlen(s1);
	int len2 = strlen(s2);
	while (index1 < len1 && index2 < len2) {
		if (s1[index1] != s2[index2]) {
			differences++;
		}
		index1++;
		index2++;
	}
	while (index1 < len1) {
		differences++;
		index1++;
	}
	while (index2 < len2) {
		differences++;
		index2++;
	}

	return differences;
}

int strspn(const char *str, const char *seps) {
	int span = 0;
	int last_char_is_seps=1;
	while (span < strlen(str) && last_char_is_seps) {
		last_char_is_seps=0;
		for (int i = 0; i < strlen(seps); i++) {
			if (str[span] == seps[i]) {
				span++;
				last_char_is_seps=1;
			}
		}
	}
	return span;
}

const char* strchr(const char *str, int character) {
	int index = 0;
	while (index < strlen(str)) {
		if (str[index] == character) {
			return str + index;
		}
		index++;
	}
	//not found
	return 0;
}
