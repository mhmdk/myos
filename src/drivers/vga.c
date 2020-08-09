#include "drivers/vga.h"
#include "ports.h"
#include"kernel_libc/string.h"

void _enable_cursor();
void _setup_crtc_address();

void init_vga() {
	_setup_crtc_address();
	_enable_cursor();
	set_cursor_position(0, 0);
}

void set_cursor_position(uint16_t x, uint16_t y) {
	uint16_t position = x * SCREEN_WIDTH + y;

	outb(CRTC_ADDRESS_PORT, CURSOR_LOCATION_LOW_REGISTER_INDEX);
	outb(CRTC_DATA_PORT, (uint8_t) (position & 0xFF));
	outb(CRTC_ADDRESS_PORT, CURSOR_LOCATION_HIGH_REGISTER_INDEX);
	outb(CRTC_DATA_PORT, (uint8_t) ((position >> 8) & 0xFF));

	//if black foreground,set it to white
	if ((SCREEN_BUFFER[position] & 0x0F00) == 0) {
		SCREEN_BUFFER[position] |= 0x0F00;
	}
}

void clear_screen() {
	memset((uint16_t*) SCREEN_BUFFER, BLACK_ON_BLACK,
			SCREEN_WIDTH * SCREEN_HIGHT * sizeof(uint16_t));
}

void _enable_cursor() {
	//http://www.osdever.net/FreeVGA/vga/textcur.htm
	//https://wiki.osdev.org/Text_Mode_Cursor

	outb(CRTC_ADDRESS_PORT, CURSOR_START_REGISTER_INDEX);
	uint8_t cursor_start_register_content = inb(CRTC_DATA_PORT);
	cursor_start_register_content &= ~CURSOR_DISABLE_BIT;
	//cursor start=0
	cursor_start_register_content &= 0xe0;  //clear least five bits
	outb(CRTC_DATA_PORT, cursor_start_register_content);

	outb(CRTC_ADDRESS_PORT, MAXIMUM_SCAN_LINE_REGISTER_INDEX);
	//maximum_scan_line in text modes is character hight -1
	uint8_t maximum_scan_line = inb(CRTC_DATA_PORT) & 0x1f; //least five bits

	outb(CRTC_ADDRESS_PORT, CURSOR_END_REGISTER_INDEX);
	uint8_t cursor_end_register_content = inb(CRTC_DATA_PORT);
	cursor_end_register_content &= (0xe0 | maximum_scan_line); //set least five bits per maximum_scan_line
	outb(CRTC_DATA_PORT, cursor_end_register_content);
}

void _setup_crtc_address() {
	//// http://www.osdever.net/FreeVGA/vga/crtcreg.htm
	//sets CRTC  IO port to 0x3D4/0x3D5
	uint8_t miscellanous_output_register = inb(
			MISCELLANOUS_OUTPUT_REGISTER_READ_PORT);
	miscellanous_output_register |= INPUT_OUTPUT_SELECT_BIT;
	outb(MISCELLANOUS_OUTPUT_REGISTER_WRITE_PORT, miscellanous_output_register);
}
