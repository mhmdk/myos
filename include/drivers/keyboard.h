#ifndef _DRIVERS_KEYBOARD_H
#define _DRIVERS_KEYBOARD_H

#include <stdint.h>
#include"keyboard_keycodes.h"

void init_keyboard();
void handle_keyboard_interrupt();

#define KEYMAP_SIZE 0x80
static const uint8_t NO_CHAR_READ = 255;

void _command_to_keyboard(uint8_t command);
uint8_t is_key_pressed(uint8_t keycode);
void set_key_pressed(uint8_t keycode);
void set_key_released(uint8_t keycode);
uint8_t is_valid_key(uint8_t keycode);
uint8_t read_keycode_from_buffer();
void add_keycode_to_buffer(uint8_t key);
uint8_t is_numlock_on();
uint8_t is_capslock_on();
uint8_t is_scrolllock_on();


static const uint8_t scancode_set1_keycode[KEYMAP_SIZE]={
	KEY_RESERVED,
    KEY_ESCAPE,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_0,
    KEY_DASH,
    KEY_EQUALS,
    KEY_BACKSPACE,
    KEY_TAB,
    KEY_Q,
    KEY_W,
    KEY_E,
    KEY_R,
    KEY_T,
    KEY_Y,
    KEY_U,
    KEY_I,
    KEY_O,
    KEY_P,
    KEY_OPEN_BRACKET,
    KEY_CLOSE_BRACKET,
    KEY_ENTER,
    KEY_LEFT_CONTROL,
    KEY_A,
    KEY_S,
    KEY_D,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_SEMICOLON,
    KEY_SINGLE_QUOTE,
    KEY_BACKTICK,
    KEY_LEFT_SHIFT,
    KEY_BACKSLASH,
    KEY_Z,
    KEY_X,
    KEY_C,
    KEY_V,
    KEY_B,
    KEY_N,
    KEY_M,
    KEY_COMMA,
    KEY_DOT,
    KEY_SLASH,
    KEY_RIGHT_SHIFT,
    KEY_KEYPAD_ASTERISK,
    KEY_LEFT_ALT,
    KEY_SPACE,
    KEY_CAPSLOCK,
    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_NUMLOCK,
    KEY_SCROLL_LOCK,
    KEY_KEYPAD_7, 
    KEY_KEYPAD_8,
    KEY_KEYPAD_9,
    KEY_KEYPAD_DASH,
    KEY_KEYPAD_4,
    KEY_KEYPAD_5,
    KEY_KEYPAD_6,
    KEY_KEYPAD_PLUS,
    KEY_KEYPAD_1,
    KEY_KEYPAD_2,
    KEY_KEYPAD_3,
    KEY_KEYPAD_0,
    KEY_KEYPAD_DOT,
    KEY_RESERVED,
    KEY_RESERVED,
    KEY_RESERVED,
    KEY_F11,
    KEY_F12,
    KEY_RESERVED,

/* 0x60 */  KEY_RESERVED, KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED, KEY_RESERVED,  KEY_RESERVED,
/* 0x68 */  KEY_RESERVED, KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED, KEY_RESERVED,  KEY_RESERVED,
/* 0x70 */  KEY_RESERVED, KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED, KEY_RESERVED,  KEY_RESERVED,
/* 0x78 */  KEY_RESERVED, KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED, KEY_RESERVED,  KEY_RESERVED
};

static const uint8_t scancode_set1_keycode_e0_prefixed[KEYMAP_SIZE]={
/* 0x00 */  KEY_RESERVED, KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED, KEY_RESERVED,  KEY_RESERVED,
/* 0x08 */  KEY_RESERVED, KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED, KEY_RESERVED,  KEY_RESERVED,
/* 0x10 */  KEY_RESERVED, KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED, KEY_RESERVED,  KEY_RESERVED,
/* 0x18 */  KEY_RESERVED, KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED,  KEY_KEYPAD_ENTER,  KEY_RESERVED, KEY_RESERVED,  KEY_RESERVED,
/* 0x20 */  KEY_RESERVED, KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED, KEY_RESERVED,  KEY_RESERVED,
/* 0x28 */  KEY_RESERVED, KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED, KEY_RESERVED,  KEY_RESERVED,
/* 0x30 */  KEY_RESERVED, KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED,  KEY_KEYPAD_SLASH, KEY_RESERVED,  KEY_RESERVED,
/* 0x38 */  KEY_RIGHT_ALT, KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED, KEY_RESERVED,  KEY_RESERVED,
/* 0x40 */  KEY_RESERVED, KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED, KEY_RESERVED,  KEY_HOME,
/* 0x48 */  KEY_UP_ARROW, KEY_PAGEUP,  KEY_RESERVED,  KEY_LEFT_ARROW,  KEY_RESERVED,  KEY_RESERVED, KEY_RIGHT_ARROW,  KEY_END,
/* 0x50 */  KEY_DOWN_ARROW, KEY_PAGEDOWN,  KEY_INSERT,  KEY_DELETE,  KEY_RESERVED,  KEY_RESERVED, KEY_RESERVED,  KEY_RESERVED,
/* 0x58 */  KEY_RESERVED, KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED, KEY_RESERVED,  KEY_RESERVED,
/* 0x60 */  KEY_RESERVED, KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED, KEY_RESERVED,  KEY_RESERVED,
/* 0x68 */  KEY_RESERVED, KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED, KEY_RESERVED,  KEY_RESERVED,
/* 0x70 */  KEY_RESERVED, KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED, KEY_RESERVED,  KEY_RESERVED,
/* 0x78 */  KEY_RESERVED, KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED,  KEY_RESERVED, KEY_RESERVED,  KEY_RESERVED
};


#endif
