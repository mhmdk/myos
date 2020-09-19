#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H

#include<stdint.h>

static const uint16_t interrupts_offset = 32;
__attribute__((interrupt)) void dummy_isr(void*);
__attribute__((interrupt)) void isr_01_keyboard(void*);
__attribute__((interrupt)) void isr_14_ata(void*);
__attribute__((interrupt)) void isr_15_ata(void*);
void enable_interrupts();

typedef struct {
	uint32_t eip;
	uint16_t cs;
	uint16_t cspadding;
	uint16_t eflags;

	// the following registers are pushed only when the handler
	// is going to be executed at numerically lower privilege
	// see Intel developer's manual volume 3 , 6.12.1
	uint16_t esp;
	uint16_t ss;
	uint16_t sspadding;
} TrapFrame;

typedef struct {
	uint32_t err;
	TrapFrame trap_frame;
} ExceptionFrame;


#endif
