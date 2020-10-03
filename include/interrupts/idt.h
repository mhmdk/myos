#ifndef __IDT_H
#define __IDT_H

#include<stdint.h>

#include "idtentry.h"

#define IDT_SIZE 256
typedef struct __attribute__((__packed__)) {

	uint16_t size;
	IdtEntry *adderess;
} InterruptDescriptorTable;

void fill_idt(InterruptDescriptorTable *idt);
void load_idt(InterruptDescriptorTable*);
void set_interrupt_handler(uint8_t interrupt_number, uint32_t function);


#endif

