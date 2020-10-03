#include "interrupts/pic.h"
#include"drivers/ports.h"

// https://pdos.csail.mit.edu/6.828/2005/readings/hardware/8259A.pdf
// https://wiki.osdev.org/PIC#Programming_the_PIC_chips

 
static const uint8_t ICW1_ICW4	=0x01	;	/* ICW4 (not) needed */
static const uint8_t ICW1_INIT	= 0x10;		/* Initialization - required! */
static const uint8_t ICW4_8086	= 0x01;		/* 8086/88 (MCS-80/85) mode */

static const uint16_t PIC1_DATA=0x21;
static const uint16_t PIC2_DATA=0xA1;
static const uint16_t PIC1_COMMAND=0x20;
static const uint16_t PIC2_COMMAND=0xA0;


/*
arguments:
	offset1 - vector offset for master PIC
		vectors on the master become offset1..offset1+7
	offset2 - same for slave PIC: offset2..offset2+7
*/
void setup_pic(uint32_t master_offset, uint32_t slave_offset){
  uint8_t pic1_masks, pic2_masks;
 
	 pic1_masks = inb(PIC1_DATA);                        // save masks
	pic2_masks = inb(PIC2_DATA);
 
	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)

	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

	outb(PIC1_DATA, master_offset);                 // ICW2: Master PIC vector offset

	outb(PIC2_DATA, slave_offset);                 // ICW2: Slave PIC vector offset

	outb(PIC1_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)

	outb(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)

 
	outb(PIC1_DATA, ICW4_8086);

	outb(PIC2_DATA, ICW4_8086);
	 
	outb(PIC1_DATA,  pic1_masks);   // restore saved masks.
	outb(PIC2_DATA, pic2_masks);
}

void acknowledge_interupt_from_master(){
  outb(PIC1_COMMAND,0x20);
}
void acknowledge_interupt_from_slave(){
  acknowledge_interupt_from_master();
  outb(PIC2_COMMAND,0x20);
  
}


