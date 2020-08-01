#include"keyboard_layout.h"
#include"keyboard_keycodes.h"
#include"drivers/keyboard.h"

char character_from_keycode(uint8_t keycode){

return unshifted_map[keycode];
}

