#include"kernel_libc/stdio.h"
#include"kernel_libc/string.h"
#include"keyboard_layout.h"
#include"drivers/keyboard.h"
#include"console.h"


char* gets(char *s) {
	int index = 0;
	char c = getchar();
	while (c != '\n') {
		s[index] = c;
		index++;
		c = getchar();
	}
	return s;
}

int puts(char*s) {
	print(s);
	print("\n");

	return strlen(s);
}

char getchar() {
	uint8_t keycode = read_keycode_from_buffer();
	if (keycode != NO_CHAR_READ) {
		char c = character_from_keycode(keycode);
		if (c == 0) {
			return keycode;
		} else {
			//printable char
			return c;
		}
	} else {
		return EOF;
	}
}
