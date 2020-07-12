#include"console.h"
void _hex_format(uint32_t number, char *s);
void _append_to_console(Console *const console, char *const s);

void print(Console *const console, char *const s) {
	_append_to_console(console, s);
	// when console has a separate buffer , copy the buffer to screen
	//currently console buffer IS THE screen , so no need for that
}

void print_hex(Console *const console, uint32_t number) {
	char s[11] = "0x00000000";
	_hex_format(number, s);
	_append_to_console(console, s);
	// when console has a separate buffer , copy the buffer to screen
	//currently console buffer IS THE screen , so no need for that
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
				SCREEN_BUFFER[console_buffer_index] = (uint16_t) (COLOR_BYTE
						<< 8) | ' ';
				console->current_column++;
			}
			console->current_row++;
			console->current_column = 0;
		} else {
			uint16_t console_buffer_index = console->current_row * SCREEN_WIDTH
					+ console->current_column;
			SCREEN_BUFFER[console_buffer_index] = (uint16_t) (COLOR_BYTE << 8)
					| s[string_index];

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
