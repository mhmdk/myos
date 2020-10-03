#include"terminal.h"
#include"drivers/keyboard_layout.h"
#include"drivers/keyboard.h"
#include"drivers/console.h"
#include"kernel_libc/string.h"
#include"kernel_libc/stdlib.h"
#include"common/stringutils.h"
#include"common/dllist.h"
#include"filesystem/filesystem.h"
#include"memory/kmalloc.h"
#include"process/elf.h"
#include"process/scheduler.h"

static const int MAX_INPUT_SIZE = 100;
void terminal_main() {
	char user_input[101] = { 0 };
	int input_next_index = 0;
	kprint("****************************\n");
	kprint("Welcome\n");
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
				//not a printable character
				//putchar('~');
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
	trim(user_input, 0);

	char command[50], arg1[50];
	next_token(command, user_input, ' ');
	trim(command, 0);
	user_input += strlen(command) + 1;
	trim(user_input, 0);

	if (strcmp(command, "ls") == 0) {
		next_token(arg1, user_input, ' ');
		List *directories = list_directory(arg1);
		dllist_for_each(directories, kprint);
		kprint("\n");
		kfree(directories);

	} else if (strcmp(command, "cat") == 0) {
		next_token(arg1, user_input, ' ');
		File *file = open_file(arg1);
		char *buffer = (char*) kmalloc(file->fat32file->size);
		read_from_file(file, buffer, file->fat32file->size, 0);
		kprint(buffer);

	} else if (strcmp(command, "exec") == 0) {
		next_token(arg1, user_input, ' ');
		Process *p = execute_elf(arg1);
		if(p!=0){
			scheduler_add_process(p);
		}
	} else if (strcmp(command, "kill") == 0) {
		next_token(arg1, user_input, ' ');
		int pid = atoi(arg1);
		kkill(pid);
	} else {
		kprint("unknown command ");
		kprint(command);
		kprint("\n");
	}
}

