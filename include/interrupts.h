#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H

#include<stdint.h>

static const uint16_t interrupts_offset=32;
__attribute__((interrupt)) void dummy_isr(void* );
__attribute__((interrupt)) void isr_01_keyboard(void* );
__attribute__((interrupt)) void isr_14_ata(void* );
__attribute__((interrupt)) void isr_15_ata(void* );
void enable_interrupts();


#endif
