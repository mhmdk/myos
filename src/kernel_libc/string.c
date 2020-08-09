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

int strlen(char *s) {
	int index = 0;
	while (s[index] != 0) {
		index++;
	}
	return index;
}
