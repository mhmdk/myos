#ifndef INCLUDE_DRIVERS_VGA_H_
#define INCLUDE_DRIVERS_VGA_H_

#include <stdint.h>

// http://www.osdever.net/FreeVGA/vga/extreg.htm#3CCR3C2W
static uint16_t MISCELLANOUS_OUTPUT_REGISTER_READ_PORT = 0x3CC;
static uint16_t MISCELLANOUS_OUTPUT_REGISTER_WRITE_PORT = 0x3C2;
static uint8_t INPUT_OUTPUT_SELECT_BIT = 1;

// http://www.osdever.net/FreeVGA/vga/crtcreg.htm
static uint16_t CRTC_ADDRESS_PORT = 0x3D4;
static uint16_t CRTC_DATA_PORT = 0x3D5;
static uint16_t MAXIMUM_SCAN_LINE_REGISTER_INDEX = 0x09;
static uint16_t CURSOR_START_REGISTER_INDEX = 0x0A;
static uint16_t CURSOR_END_REGISTER_INDEX = 0x0B;
static uint8_t CURSOR_DISABLE_BIT = 1 << 5;
static uint16_t CURSOR_LOCATION_HIGH_REGISTER_INDEX = 0x0E;
static uint16_t CURSOR_LOCATION_LOW_REGISTER_INDEX = 0x0F;


static uint16_t *const SCREEN_BUFFER = (uint16_t*) 0xB8000;
static uint32_t const SCREEN_HIGHT = 25;
static uint32_t const SCREEN_WIDTH = 80;

// https://wiki.osdev.org/Text_UI
//will ignore 7th bit for now
static uint8_t const WHITE = 15;
static uint8_t const BLACK = 0;
static uint8_t const WHITE_ON_BLACK = WHITE | (BLACK << 4);
static uint8_t const BLACK_ON_BLACK = BLACK | (BLACK << 4);

void init_vga();
void set_cursor_position(uint16_t x, uint16_t y);
void clear_screen();

#endif /* INCLUDE_DRIVERS_VGA_H_ */
