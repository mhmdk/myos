#include"console.h"

void print(Console *const console, char *const s) {
	append_to_console(console, s);
	// when console has a separate buffer , copy the buffer to screen
	//currently console buffer IS THE screen , so no need for that
}

void append_to_console(Console *const console, char *const s) {
	uint16_t string_index = 0;

	while (s[string_index] != 0) {

		if (s[string_index] == '\n') {
			console->current_row++;
			console->current_column=0;
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
