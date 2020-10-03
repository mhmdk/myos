#include"keyboard_layout.h"
#include"keyboard_keycodes.h"
#include"drivers/keyboard.h"

uint8_t _is_keypad_key(uint8_t keycode);
uint8_t _is_shifted();

static char unshifted_map[]= {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	'`', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0, 0, 0, 0, 0, 0, '/', '*', '-', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	'\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\\',0, 0, 0, 0, 0, 0,'+',0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0,
	0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '\n',0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, 0, 0, 0, 0, 0, 0, 0, 0,'\n',0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static char shifted_map[]= {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	'~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0, 0, 0, 0, 0, 0, '/', '*', '-', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	'\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '|', 0, 0, 0, '7', '8', '9','+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '\n', 0, 0, 0, 0, '4', '5', '6', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, 0, 0, 0, 0, 0, '1', '2', '3', '\n', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '0', 0, '.', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

char character_from_keycode(uint8_t keycode) {
	uint8_t use_shifted_map;
	if (_is_keypad_key(keycode)) {
		use_shifted_map =is_numlock_on() ;
	} else {
		use_shifted_map = _is_shifted();
	}

	if (use_shifted_map) {
		return shifted_map[keycode];
	}
	return unshifted_map[keycode];
}

uint8_t _is_keypad_key(uint8_t keycode) {
	uint8_t column = keycode & 0x1f; //least five bits
	return column > 16;
}
uint8_t _is_shifted() {
	return is_capslock_on()
			^ (is_key_pressed(KEY_LEFT_SHIFT) | is_key_pressed(KEY_RIGHT_SHIFT));
}
