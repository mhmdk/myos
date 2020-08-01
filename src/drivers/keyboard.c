#include"drivers/keyboard.h"
#include <stdint.h>
#include"console.h"
#include"ports.h"
#include"common/bitmap.h"
#include"kmalloc.h"
//TODO this should be removed , it was used for tracing purposes
#include"keyboard_layout.h"

BitMap keyboard_state;
uint8_t numlock = 0;
uint8_t capslock = 0;
uint8_t scrolllock = 0;
uint8_t *keyboard_buffer;

static const uint16_t ps2_controller_data_port = 0x60;
static const uint16_t ps2_controller_command_port = 0x64;

static uint8_t e0_prefix = 0;
static uint8_t _bitmap_data[256 / 8] = { 0 };
static const uint32_t BUFFER_SIZE = 1024 * 16;
static uint32_t buffer_read_index = 0;
static uint32_t buffer_write_index = 0;

void _toggle_lock_states(uint8_t keycode);
void _command_to_keyboard(uint8_t command);

void init_keyboard() {
	outb(ps2_controller_command_port, 0xAD);		//disable first PS/2 port
	outb(ps2_controller_command_port, 0xA7);		//disable  second PS/2 port

	uint8_t ans = inb(ps2_controller_data_port);		//flush output port

	outb(ps2_controller_command_port, 0xAA);
	ans = inb(ps2_controller_data_port); //should be 0x55

	outb(ps2_controller_command_port, 0x20); //read the state
	uint8_t state = inb(ps2_controller_data_port);
	state |= 1; //enable interrupts from  first port
	state &= ~(1 << 5); //disable interrupts from second port
	state &= ~(1 << 6); //disable translation
	outb(ps2_controller_command_port, 0x60); // set the state
	outb(ps2_controller_data_port, state);

	_command_to_keyboard(0xF4); //enable scanning
	ans = inb(ps2_controller_data_port); //0xFA (ACK) or 0xFE (Resend)

	_command_to_keyboard(0xf0); //set keyboard set
	_command_to_keyboard(0x01);
	ans = inb(ps2_controller_data_port); //0xFA (ACK) or 0xFE (Resend)

	outb(ps2_controller_command_port, 0xAE); //enable  first PS/2 port

	keyboard_state = bitmap_wrap_data(_bitmap_data, 256 / 8);
	keyboard_buffer = (uint8_t*) kmalloc(BUFFER_SIZE);
}

void handle_keyboard_interrupt() {
	Console console = { 5, 0 };
	set_active_console(&console);

	uint8_t scancode = inb(ps2_controller_data_port);
	uint8_t keycode;
	if (scancode == 0xE0) {
		e0_prefix = 1;
	} else {
		e0_prefix = 0;
		uint8_t pressed = !(scancode & 0x80);
		scancode &= 0x7f; //remove the released flag
		if (scancode >= KEYMAP_SIZE) {
			//TODO log a warning, this should not happen
			return;
		}
		if (e0_prefix) {
			keycode = scancode_set1_keycode_e0_prefixed[scancode];
		} else {
			keycode = scancode_set1_keycode[scancode];
		}

		if (pressed) {
			print(&console, "pressed\n");
			print_hex(&console, scancode);
			print(&console, "\n");
			print_hex(&console, keycode);
			print(&console, "\n");
			char s[2] = "0";
			s[0] = character_from_keycode(keycode);
			print(&console, s);

		} else {
			print(&console, "released\n");
			print_hex(&console, scancode);
			print(&console, "\n");
			print_hex(&console, keycode);
		}
		if (is_valid_key(keycode)) {
			if (pressed) {
				set_key_pressed(keycode);
				add_keycode_to_buffer(keycode);
				_toggle_lock_states(keycode);
			} else {
				set_key_released(keycode);
			}
		}
	}
}

uint8_t is_key_pressed(uint8_t keycode) {
	return bitmap_get(&keyboard_state, keycode);
}

void set_key_pressed(uint8_t keycode) {
	bitmap_set(&keyboard_state, keycode);
}

void set_key_released(uint8_t keycode) {
	bitmap_unset(&keyboard_state, keycode);
}

uint8_t is_valid_key(uint8_t keycode) {
	return keycode != KEY_RESERVED;
}

uint8_t read_keycode_from_buffer() {
	if (buffer_read_index == buffer_write_index) {
		return -1; //nothing to read
	}
	char c = keyboard_buffer[buffer_read_index];
	++buffer_read_index;
	return c;
}

void add_keycode_to_buffer(uint8_t key) {
	keyboard_buffer[buffer_write_index] = key;
	buffer_write_index++;
	if (buffer_write_index >= BUFFER_SIZE) {
		buffer_write_index %= BUFFER_SIZE;
	}
}
uint8_t is_numlock_on(){
	return numlock;
}
uint8_t is_capslock_on(){
	return capslock;
}
uint8_t is_scrolllock_on(){
	return scrolllock;
}


void _toggle_lock_states(uint8_t keycode) {
	switch (keycode) {
	case KEY_CAPSLOCK:
		capslock ^= 1;
		break;
	case KEY_NUMLOCK:
		numlock ^= 1;
		break;
	case KEY_SCROLL_LOCK:
		scrolllock ^= 1;
	}
}

void _command_to_keyboard(uint8_t command) {
	while (inb(ps2_controller_command_port) & 0x2) {
		//bit 1 should be clear (input buffer status is empty) before writing
		//waiting
	}
	outb(ps2_controller_data_port, command);
}

