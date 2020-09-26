#include"terminal.h"
#include"keyboard_layout.h"
#include"drivers/keyboard.h"
#include"console.h"
#include"kernel_libc/string.h"
#include"common/stringutils.h"

static const int MAX_INPUT_SIZE = 100;
void terminal_main() {
	char user_input[101] = { 0 };
	int input_next_index = 0;
	while (1) {
		uint8_t keycode = read_keycode_from_buffer();

		if (keycode == NO_CHAR_READ) {
			continue;
		} else if (keycode == KEY_PAGEUP || keycode == KEY_KEYPAD_9) {
			page_up();
		} else if (keycode == KEY_PAGEDOWN || keycode == KEY_KEYPAD_3) {
			page_down();
		} else if (keycode == KEY_DOWN_ARROW || keycode == KEY_KEYPAD_2) {
			scroll_down(1);
		} else if (keycode == KEY_UP_ARROW || keycode == KEY_KEYPAD_8) {
			scroll_up(1);
		} else if (keycode == KEY_BACKSPACE) {
			if (input_next_index > 0) {
				input_next_index--;
				user_input[input_next_index] = '\0';
				scroll_to_cursor();
				erase_char();

			}
		} else if (keycode == KEY_ENTER || keycode == KEY_KEYPAD_ENTER) {
			scroll_to_cursor();
			kprint("\n");

			process_input(user_input);

			input_next_index = 0;
			user_input[0] = '\0';
		} else {
			char c = character_from_keycode(keycode);
			if (c == 0) {
				scroll_to_cursor();
				putchar('~');
			} else {
				//printable char
				if (input_next_index <= MAX_INPUT_SIZE) {
					user_input[input_next_index] = c;
					input_next_index++;
					user_input[input_next_index] = '\0';
				}
				scroll_to_cursor();
				putchar(c);
			}
		}
	}

}

void process_input(char user_input[]) {
	trim(user_input,0);

	if (strcmp(user_input, "pm") == 0) {
		kprint("memory map\n");
	} else {
		kprint("unknown command\n");
	}
}

