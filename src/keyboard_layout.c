#include"keyboard_layout.h"
#include"keyboard_keycodes.h"
#include"drivers/keyboard.h"

uint8_t _is_keypad_key(uint8_t keycode);
uint8_t _is_shifted();

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
