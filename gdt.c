#include"gdt.h"

void __fill_null_selector(GdtEntry*);
void __fill_code_selector(GdtEntry*);
void __fill_data_selector(GdtEntry*);

void load_gdt(GlobalDescriptorTable* gdt){
  asm("lgdt (%0)"::"p"(gdt));
}

GlobalDescriptorTable make_gdt(){
  GlobalDescriptorTable gdt;
  
  __fill_null_selector(&(gdt.null_segment));
  __fill_code_selector(&(gdt.code_segment));
  __fill_data_selector(&(gdt.data_segment));
  __fill_null_selector(&(gdt.extra_segment));
  __fill_null_selector(&(gdt.tss_segment));

 
  gdt.gdt_descriptor.adderess=&(gdt.null_segment);
  //size = 8*N -1 where 8 is the size of one descriptor
  gdt.gdt_descriptor.size=sizeof(GlobalDescriptorTable)-sizeof(GDTDescriptor) -1; 
  
  return gdt;
}

void __fill_null_selector(GdtEntry* entry){
  fill_gdt_entry(entry,0,0,0);
}

void __fill_code_selector(GdtEntry* entry){
  uint8_t flags = PRESENT | CODE_OR_DATA | EXECUTABLE | READABLE_OR_WRITABLE;
  fill_gdt_entry(entry,0,0xffffffff,flags);
}

void __fill_data_selector(GdtEntry* entry){
  uint8_t flags =  PRESENT | CODE_OR_DATA | READABLE_OR_WRITABLE;;
  fill_gdt_entry(entry,0,0xfffffff,flags);
}
