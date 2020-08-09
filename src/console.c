#include"console.h"
#include"kmalloc.h"
#include"drivers/vga.h"
#include"kernel_libc/string.h"

void _hex_format(uint32_t number, char *s);
void _append_to_console(char *const s);
int _get_console_buffer_index(Console*);
void _copy_to_screen();
void _increment_row();
void _increment_column();
void _check_last_column();
void _check_last_row();
uint16_t _make_vga_char(char c);
int _buffer_beginning();
int _circular_diff(int beginning, int end, int modulus);

static Console *active_console = 0;

void init_console() {
	active_console = (Console*) kmalloc(sizeof(Console));

	active_console->number_of_columns = SCREEN_WIDTH;
	//TODO why does allocating14 pages fail? kmalloc issue?
	active_console->number_of_rows = SCREEN_HIGHT * 3;

	active_console->buffer = (uint16_t*) kmalloc(
			(uint32_t)active_console->number_of_rows * active_console->number_of_columns
					* sizeof(uint16_t));

	clear_console();
	clear_screen();
}

void print(char *const s) {
	_append_to_console(s);
	_copy_to_screen();
}

void putchar(char c) {
	char s[2] = { 0, 0 };
	s[0] = c;
	print(s);
}

void clear_console() {
	memset(active_console->buffer, BLACK_ON_BLACK,
			active_console->number_of_rows * active_console->number_of_columns
					* sizeof(uint16_t));
	active_console->output_column = 0;
	active_console->output_row = 0;
	active_console->cursor_row = 0;
	active_console->cursor_column = 0;
	active_console->used_rows = 1;
}

void print_hex(uint32_t number) {
	char s[11] = "0x00000000";
	_hex_format(number, s);
	print(s);
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

void page_up() {
	scroll_up(SCREEN_HIGHT);
}
void page_down() {
	scroll_down(SCREEN_HIGHT);
}

void scroll_down(int rows) {
	int max_possible_scroll = _circular_diff(active_console->output_row,
			active_console->cursor_row, active_console->number_of_rows);
	if (rows > max_possible_scroll) {
		rows = max_possible_scroll;
	}
	int target_row = (active_console->output_row + rows)
			% active_console->number_of_rows;

	scroll_to_row(target_row);
}

void scroll_up(int rows) {
	int beginning = _buffer_beginning();
	int max_possible_scroll = _circular_diff(beginning-1,
			active_console->output_row, active_console->number_of_rows);
	//because beginning at the top, where output_row is at the buttom
	max_possible_scroll -= SCREEN_HIGHT;
	if (max_possible_scroll <= 0) {
		return;
	}
	if (rows > max_possible_scroll) {
		rows = max_possible_scroll;
	}
	int target_row = active_console->output_row - rows;
	if (target_row < 0) {
		target_row += active_console->number_of_rows;
	}
	scroll_to_row(target_row);
}

void scroll_to_cursor() {
	scroll_to_row(active_console->cursor_row);
}

void scroll_to_row(int row) {
	active_console->output_row = row;
	_copy_to_screen();
}

void _copy_to_screen() {

	int console_buffer_end_index = (active_console->output_row + 1)
			% active_console->number_of_rows;

	int rows_to_copy = 0;
	if (active_console->used_rows < SCREEN_HIGHT) {
		rows_to_copy = active_console->used_rows;
	} else {
		rows_to_copy = SCREEN_HIGHT;
	}
	int console_buffer_start_index = console_buffer_end_index - rows_to_copy;
	if (console_buffer_start_index < 0) {
		console_buffer_start_index += active_console->number_of_rows;
	}
	int screen_row = 0;
	int buffer_row = console_buffer_start_index;
	int copied_rows = 0;

	while (copied_rows < rows_to_copy) {
		memcpy(SCREEN_BUFFER + screen_row * SCREEN_WIDTH,
				active_console->buffer + buffer_row * SCREEN_WIDTH,
				SCREEN_WIDTH * sizeof(uint16_t));
		if (buffer_row == active_console->cursor_row) {
			set_cursor_position(screen_row, active_console->cursor_column);
		}

		copied_rows++;
		screen_row++;
		buffer_row++;
		if (buffer_row >= active_console->number_of_rows) {
			buffer_row = 0;
		}
	}

}
void _append_to_console(char *const s) {
	uint16_t string_index = 0;
	while (s[string_index] != 0) {

		if (s[string_index] == '\n') {
			_increment_row();
			active_console->output_column = 0;
		} else if (s[string_index] == '\t') {
			_append_to_console("    ");
		} else {
			int console_buffer_index = _get_console_buffer_index(
					active_console);
			active_console->buffer[console_buffer_index] = _make_vga_char(
					s[string_index]);
			_increment_column();
		}
		string_index++;
	}
	active_console->cursor_column = active_console->output_column;
	active_console->cursor_row = active_console->output_row;
}

void _increment_column() {
	active_console->output_column++;
	_check_last_column();
}

void _increment_row() {
	active_console->output_row++;

	if (active_console->used_rows < active_console->number_of_rows) {
		active_console->used_rows++;
	}
	_check_last_row();
}

void _check_last_column() {
	if (active_console->output_column >= SCREEN_WIDTH) {
		active_console->output_column %= SCREEN_WIDTH;
		_increment_row();
	}
}

void _check_last_row() {
	if (active_console->output_row >= active_console->number_of_rows) {
		active_console->output_row = 0;
	}
}

uint16_t _make_vga_char(char c) {
	return (((uint16_t) WHITE_ON_BLACK) << 8) | c;
}

int _get_console_buffer_index(Console *active_console) {
	return active_console->output_row * active_console->number_of_columns
			+ active_console->output_column;
}

int _buffer_beginning() {
	return _circular_diff(active_console->cursor_row, active_console->used_rows-1,
			active_console->number_of_rows);
}

int _circular_diff(int beginning, int end, int modulus) {
	int diff = end - beginning;
	if (diff < 0) {
		diff += modulus;
	}
	return diff;
}
