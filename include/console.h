#ifndef  _CONSOLE_H
#define _CONSOLE_H

#include<stdint.h>
// https://wiki.osdev.org/Printing_To_Screen

static uint16_t *const SCREEN_BUFFER = (uint16_t*) 0xB8000;
static uint8_t const SCREEN_HIGHT = 25;
static uint8_t const SCREEN_WIDTH = 80;

// https://wiki.osdev.org/Text_UI
//will ignore 7th bit for now
static uint8_t const FOREGROUD_COLOR = 15; //white
static uint8_t const BACKGROUDN_COLOR = 0; //black
static uint8_t const COLOR_BYTE = FOREGROUD_COLOR | (BACKGROUDN_COLOR << 4);

typedef struct {
	uint8_t current_row;
	uint8_t current_column;

//should allocate a console buffer , but we need memory manager
// so we stick for screen_buffer for now

} Console;

void print( Console* const,  char* const);
void print_hex(Console *const console, uint32_t number) ;

#endif
