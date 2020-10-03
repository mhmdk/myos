#ifndef __GDT_H
#define __GDT_H

#include<stdint.h>
#include"gdtentry.h"
//segment selector is: RPL(2bits),Table Indicator(1 bit whether to use gdt or ldt), index(13 bits)
// so the index is shifted by 3
// see intel's manual chapter 3.4.2
static const uint16_t code_segment_selector = 1<<3;
static const uint16_t data_segment_selector =2<<3;
static const uint16_t tss_segment_selector = 0| (3<<3);
static const uint16_t usermode_code_segment_selector = (4<<3)|3;
static const uint16_t usermode_data_segment_selector = (5<<3)|3;

typedef struct __attribute__((__packed__)) {

	uint16_t size;
	GdtEntry *adderess;
} GDTDescriptor;

typedef struct {

	GDTDescriptor gdt_descriptor;
	//best be aligned on 8 byte boundary, per intel's manual
	GdtEntry __attribute__((aligned(8))) null_segment;
	GdtEntry code_segment;
	GdtEntry data_segment;
	GdtEntry tss_segment;
	GdtEntry usermode_code_segment;
	GdtEntry usermode_data_segment;
	GdtEntry extra_segment;

} GlobalDescriptorTable;

typedef struct __attribute__((__packed__)) {
	uint32_t prev_tss;
	uint32_t esp0;
	uint32_t ss0;
	uint32_t esp1;
	uint32_t ss1;
	uint32_t esp2;
	uint32_t ss2;
	uint32_t cr3;
	uint32_t eip;
	uint32_t eflags;
	uint32_t eax;
	uint32_t ecx;
	uint32_t edx;
	uint32_t ebx;
	uint32_t esp;
	uint32_t ebp;
	uint32_t esi;
	uint32_t edi;
	uint32_t es;
	uint32_t cs;
	uint32_t ss;
	uint32_t ds;
	uint32_t fs;
	uint32_t gs;
	uint32_t ldt;
	uint16_t trap;
	uint16_t iomap_base;
} TaskStateSegment;

void fill_gdt(GlobalDescriptorTable*);
void load_gdt(GlobalDescriptorTable*);
void load_tss();
void set_tss_stack(uint32_t esp0);

#endif

