#ifndef  _CONSOLE_H
#define _CONSOLE_H

#include<stdint.h>
// https://wiki.osdev.org/Printing_To_Screen

static uint16_t *const SCREEN_BUFFER = (uint16_t*) 0xB8000;
static uint32_t const SCREEN_HIGHT = 25;
static uint32_t const SCREEN_WIDTH = 80;

// https://wiki.osdev.org/Text_UI
//will ignore 7th bit for now
static uint8_t const WHITE = 15;
static uint8_t const BLACK = 0;
static uint8_t const WHITE_ON_BLACK = WHITE | (BLACK << 4);
static uint8_t const BLACK_ON_BLACK = BLACK | (BLACK << 4);

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
