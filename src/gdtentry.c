#include"gdtentry.h"

static void  _set_base(GdtEntry* entry, uint32_t base);
static void  _set_limit(GdtEntry* entry, uint32_t limit);
static void _set_flags(GdtEntry* entry,  uint8_t flags);

void fill_gdt_entry(GdtEntry *entry,uint32_t base, uint32_t limit, uint8_t flags){

  const uint8_t SZ_32BIT_PRTECTED_MODE=1<<6;
  entry->limit_16_19_and_flags=0;
  entry->limit_16_19_and_flags|=SZ_32BIT_PRTECTED_MODE;
  
  _set_base(entry,base);
  _set_limit(entry,limit);
  _set_flags(entry,flags);
}

void fill_gdt_tss_entry(GdtEntry *entry,uint32_t base, uint32_t limit){
	//intel manual ch3, 7.2.2 : Type bits of tss should be 1001
	uint8_t flags = 0x09;
	 flags |= GDT_PRESENT;

  entry->limit_16_19_and_flags=0;

  _set_base(entry,base);
  _set_limit(entry,limit);
  _set_flags(entry,flags);
}

static void _set_base(GdtEntry* entry, uint32_t base){
  entry->base_0_15=(base&0xffff);
  entry->base_16_23=((base>>16)&0xff);
  entry->base_24_31=((base>>24)&0xff);
}

static void _set_limit(GdtEntry* entry, uint32_t limit){
  const uint32_t PAGESIZE=0x10000;//4k
  const uint8_t PAGE_GRANULARITY=1<<7;
  if(limit > UINT16_MAX){
    if((limit &(PAGESIZE-1)) !=PAGESIZE-1){
      //make the limit the closest multiple of pagesize smaller than requested
      limit|=(PAGESIZE-1);
      limit>>=12;
      limit-=1;
    }
    else{
      limit>>=12;
    }
    entry->limit_16_19_and_flags|=PAGE_GRANULARITY;
    
    uint8_t flags=entry->limit_16_19_and_flags&0xf0;
    entry->limit_16_19_and_flags=(limit>>16 |flags);
  }
  else{
    //byte granularity
    entry->limit_16_19_and_flags&=(0xff ^ PAGE_GRANULARITY);
  }
  entry->limit_0_15=(limit & 0xffff);
    
}

static void _set_flags(GdtEntry* entry,  uint8_t flags){
  entry->access_byte=flags;
}
