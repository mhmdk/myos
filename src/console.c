#include"console.h"
#include"kmalloc.h"
#include"drivers/vga.h"
#include"kernel_libc/string.h"

void _hex_format(uint32_t number, char *s);
void _append_to_console(Console *const console, char *const s);
void _copy_to_screen(Console *const console);

static Console *active_console = 0;

Console new_console() {
	Console console;
	console.current_column = 0;
	console.current_row = 0;
	console.buffer = (uint16_t*) kmalloc(
			SCREEN_HIGHT * SCREEN_WIDTH * sizeof(uint16_t));
	memset(console.buffer, BLACK_ON_BLACK,
			SCREEN_HIGHT * SCREEN_WIDTH * sizeof(uint16_t));
	return console;
}

void set_active_console(Console *console) {
	active_console = console;
}
void print_to_current_active(char *const s) {
	print(active_console, s);
}

void print(Console *const console, char *const s) {
	_append_to_console(console, s);

	if (active_console == console) {
		_copy_to_screen(console);
	}
}

void putch(Console *const console, char c) {
	char s[2] = { 0, 0 };
	s[0] = c;
	print(console, s);
}

void print_hex(Console *const console, uint32_t number) {
	char s[11] = "0x00000000";
	_hex_format(number, s);
	print(console, s);
}
void _hex_format(uint32_t number, char *s) {
	char *alphabet = "0123456789ABCDEF";
	uint8_t index = 2;
	uint8_t shift = 28;
	while (index < 10) {
		s[index] = alphabet[(number >> shift) & 0xF];
		index++;
		shift -= 4;
	}
}

void _append_to_console(Console *const console, char *const s) {
	uint16_t string_index = 0;

	while (s[string_index] != 0) {

		if (s[string_index] == '\n') {
			while (console->current_column < SCREEN_WIDTH) {
				uint16_t console_buffer_index = console->current_row
						* SCREEN_WIDTH + console->current_column;
				console->buffer[console_buffer_index] =
						(uint16_t) (WHITE_ON_BLACK << 8) | ' ';
				console->current_column++;
			}
			console->current_row++;
			console->current_column = 0;
		} else if (s[string_index] == '\t') {
			_append_to_console(console, "    ");
		} else {
			uint16_t console_buffer_index = console->current_row * SCREEN_WIDTH
					+ console->current_column;
			console->buffer[console_buffer_index] = (uint16_t) (WHITE_ON_BLACK
					<< 8) | s[string_index];

			console->current_column++;
			if (console->current_column >= SCREEN_WIDTH) {
				console->current_column %= SCREEN_WIDTH;
				console->current_row++;
			}
		}

		if (console->current_row >= SCREEN_HIGHT) {
			console->current_row = 0;
		}
		string_index++;
	}
}

void _copy_to_screen(Console *const console) {
	for (uint32_t row = 0; row < SCREEN_HIGHT; row++) {
		for (uint32_t column = 0; column < SCREEN_WIDTH; column++) {
			uint32_t index = row * SCREEN_WIDTH + column;
			SCREEN_BUFFER[index] = console->buffer[index];
		}
	}
}
