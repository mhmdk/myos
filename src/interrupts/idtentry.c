#include "interrupts/idtentry.h"

#include "memory/gdt.h"

static void  _set_offset(IdtEntry* entry, uint32_t offset);

static void _set_flags(IdtEntry* entry,  uint8_t flags, GateType type);

void fill_idt_entry(IdtEntry *entry, uint32_t offset, GateType type, uint8_t flags){
  entry->zeros=0;
  entry->segment=code_segment_selector;
  
  _set_offset(entry,offset);
  _set_flags(entry,flags,type);
}

static void _set_offset(IdtEntry* entry, uint32_t offset){
  entry->offset_0_15=(offset&0xffff);
  entry->offset_16_31=(offset>>16);
}

static void _set_flags(IdtEntry* entry,  uint8_t flags, GateType type){
  entry->flags=flags|type;
}
