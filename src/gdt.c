#include"gdt.h"

void __fill_null_selector(GdtEntry*);
void __fill_code_selector(GdtEntry*);
void __fill_data_selector(GdtEntry*);
void __reload_data_segment_registers();
void load_gdt(GlobalDescriptorTable* gdt){
  __asm__("lgdt (%0)"::"p"(gdt));

  //this jump will reload the CS register
  __asm__("ljmp %0,%1"::"n"(code_segment_selector),"p"(__reload_data_segment_registers));
}

void fill_gdt(GlobalDescriptorTable *gdt){

  
  __fill_null_selector(&(gdt->null_segment));
  __fill_code_selector(&(gdt->code_segment));
  __fill_data_selector(&(gdt->data_segment));
  __fill_null_selector(&(gdt->extra_segment));
  __fill_null_selector(&(gdt->tss_segment));

 
  gdt->gdt_descriptor.adderess=&(gdt->null_segment);
  //size = 8*N -1 where 8 is the size of one descriptor
  gdt->gdt_descriptor.size=sizeof(GlobalDescriptorTable)-sizeof(GDTDescriptor) -1; 
  
}

void __fill_null_selector(GdtEntry* entry){
  fill_gdt_entry(entry,0,0,0);
}

void __fill_code_selector(GdtEntry* entry){
  uint8_t flags = GDT_PRESENT | CODE_OR_DATA | EXECUTABLE | READABLE_OR_WRITABLE;
  fill_gdt_entry(entry,0,0xffffffff,flags);
}

void __fill_data_selector(GdtEntry* entry){
  uint8_t flags =  GDT_PRESENT | CODE_OR_DATA | READABLE_OR_WRITABLE;;
  fill_gdt_entry(entry,0,0xfffffff,flags);
}

void __reload_data_segment_registers(){
  //per multiboot specifications those segments have offset 0 originally
  //so we are not really changing anything
  //otherwise we have to deal with a messed up stack??
	//TODO this is causing if compiler option  -O0  is specified
  __asm__("push %%EAX\n\t"
	  "mov %0,%%AX\n\t"
	  "mov %%AX,%%DS\n\t"
	  "mov %%AX,%%ES\n\t"
	  "mov %%AX,%%FS\n\t"
	  "mov %%AX,%%GS\n\t"
	  "mov %%AX,%%SS\n\t"
	  "pop %%EAX"::"n"(data_segment_selector));

 // __asm__("nop");
}
