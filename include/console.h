#ifndef  _CONSOLE_H
#define _CONSOLE_H

#include<stdint.h>
// https://wiki.osdev.org/Printing_To_Screen


typedef struct {
	uint8_t current_row;
	uint8_t current_column;
	uint16_t *buffer;

} Console;

Console new_console();
void print_to_current_active( char* const);
void print(Console* const, char* const);
void print_hex(Console *const console, uint32_t number);
void set_active_console(Console*);
void putch(Console *const console, char c);


#endif
