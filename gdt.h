#ifndef __GDT_H
#define __GDT_H

#include<stdint.h>
#include"gdtentry.h"

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

GlobalDescriptorTable make_gdt();
void load_gdt(GlobalDescriptorTable*);


#endif
  
