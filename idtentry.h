 #ifndef __IDTENTRY_H
#define __IDTENTRY_H

#include<stdint.h>

//see intel developers manual , volume 3, chapter 6.10
static const uint8_t IDT_PRESENT=1<<7;
static const uint8_t IDT_PRIVILEGE_HIGH=1<<6;
static const uint8_t IDT_PRIVILEGE_LOW=1<<5;
typedef enum  {
	       interrupt_gate = 0xE,
	       trap_gate = 0xF,
	       task_gate=0x5} GateType; 


typedef struct  __attribute__((__packed__)) {
  uint16_t offset_0_15;
  uint16_t segment;
  uint8_t zeros;
  uint8_t flags;
  uint16_t offset_16_31;
} IdtEntry;

void fill_idt_entry(IdtEntry *entry, uint32_t offset, GateType type, uint8_t flags);



#endif
