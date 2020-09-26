#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H

#include<stdint.h>

#define  interrupts_offset 32 //macro because it is used in a switch statement
#define number_of_interrupts_per_pic  7
#define syscall_trap_number  128

uint64_t get_time_since_boot();

typedef struct {
	// registers  pushed pushed manually by pusha
	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t old_esp; // esp before pusha instruction : https://c9x.me/x86/html/file_module_x86_id_270.html
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;

	// pushed manually by interrupt stub
	uint16_t gs;
	uint16_t gs_padding;
	uint16_t fs;
	uint16_t fs_padding;
	uint16_t es;
	uint16_t es_padding;
	uint16_t ds;
	uint16_t ds_padding;
	uint32_t trap_number;

	// below are pushed by hardware

	uint32_t err; //pushed by hardware in case of exception,otherwise we push it with value=0

	uint32_t eip;
	uint16_t cs;
	uint16_t cs_padding;
	uint32_t eflags;

	// the following registers are pushed only when the handler
	// is going to be executed at numerically lower privilege
	// see Intel developer's manual volume 3 , 6.12.1
	uint32_t esp;
	uint16_t ss;
	uint16_t ss_padding;
} TrapFrame;

void enable_interrupts();
void disable_interrupts();
void handle_interrupt(TrapFrame *trap_frame);


#endif
