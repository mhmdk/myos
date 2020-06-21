#ifndef __GDT_H
#define __GDT_H

#include<stdint.h>
#include"gdtentry.h"
//segment selector is some flags + index, see intel's manual chapter 3.4.2
static const uint16_t code_segment_selector=0x8;
static const uint16_t data_segment_selector=0x10;
static const uint16_t tss_segment_selector=0x18;

typedef struct __attribute__((__packed__)) {
 
  uint16_t size;
  GdtEntry* adderess;
}GDTDescriptor;

typedef struct {
 
  GDTDescriptor gdt_descriptor;
   //best be aligned on 8 byte boundary, per intel's manual 
  GdtEntry __attribute__((aligned(8))) null_segment;
  GdtEntry code_segment;
  GdtEntry data_segment;
  GdtEntry tss_segment;
  GdtEntry extra_segment;
  
} GlobalDescriptorTable;

void fill_gdt(GlobalDescriptorTable*);
void load_gdt(GlobalDescriptorTable*);


#endif
  
