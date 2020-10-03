#ifndef  _CONSOLE_H
#define _CONSOLE_H

#include<stdint.h>
// https://wiki.osdev.org/Printing_To_Screen

typedef struct {
	uint16_t number_of_rows;
	uint16_t number_of_columns;
	uint16_t used_rows;
	uint16_t output_row;
	uint16_t output_column;
	uint16_t cursor_row;
	uint16_t cursor_column;
	uint16_t *buffer;

} Console;

void init_console();
void kprint(char* const);
void erase_char();
void kprint_hex(uint32_t number);
void putchar(char c);
void clear_console();
void page_up();
void page_down();
void scroll_down(int rows);
void scroll_up(int rows);
void scroll_to_cursor();
void scroll_to_row(int row);

#endif
