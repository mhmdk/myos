#ifndef __GDTENTRY_H
#define __GDTENTRY_H

#include<stdint.h>

//see intel developers manual , volume 3, chapter 3.3
static const uint8_t GDT_PRESENT=1<<7;
static const uint8_t GDT_PRIVILEGE_HIGH=1<<6;
static const uint8_t GDT_PRIVILEGE_LOW=1<<5;
static const uint8_t CODE_OR_DATA=1<<4;
static const uint8_t EXECUTABLE=1<<3;
static const uint8_t DIRECTION_CONFORMING=1<<2;
static const uint8_t READABLE_OR_WRITABLE=1<<1;

typedef struct  __attribute__((__packed__)) {
  uint16_t limit_0_15;
  uint16_t base_0_15;
  
  uint8_t base_16_23;
  uint8_t access_byte;
  uint8_t limit_16_19_and_flags;
  uint8_t base_24_31;
} GdtEntry;

void fill_gdt_entry(GdtEntry *entry, uint32_t base, uint32_t limit, uint8_t flags);
void fill_gdt_tss_entry(GdtEntry *entry,uint32_t base, uint32_t limit);



#endif
