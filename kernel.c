#include<stdint.h>
#include<stddef.h>
#include "multiboot.h"
#include "gdt.h"
#include "idt.h"
#include "interrupts.h"
#include "pic.h"

#if defined(__linux__)
#error "compiling for linux"
#endif 
 
#if !defined(__i386__)
#error "not i386"
#endif

uint16_t* const terminal_buffer=(uint16_t*) 0xB8000;

void print(char c){
  for(int i=0;i<30;i++)terminal_buffer[i] = (uint16_t)c | (uint16_t)(15<<8);
}

void kernel_main(multiboot_uint32_t magic,multiboot_info_t* multibootinfo ){
  unsigned char c;
  if(magic == MULTIBOOT_BOOTLOADER_MAGIC){
    c='a';
   }
   else{
     c='c';
   }
  GlobalDescriptorTable gdt;
  fill_gdt(&gdt);
  load_gdt(&gdt);


  InterruptDescriptorTable idt;
  fill_idt(&idt);
  load_idt(&idt);

  setup_pic(interrupts_offset,interrupts_offset+8);
  enable_interrupts();

  print(c);
 
  while(1){
    __asm__ ("hlt");
  }
  
}
