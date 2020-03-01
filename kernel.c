#include<stdint.h>
#include<stddef.h>
#include "multiboot.h"

#if defined(__linux__)
#error "compiling for linux"
#endif 
 
#if !defined(__i386__)
#error "not i386"
#endif

uint16_t* const terminal_buffer=(uint16_t*) 0xB8000;
void kernel_main(multiboot_uint32_t* magic,multiboot_info_t* multibootinfo ){
  unsigned char c;
  if(*magic == MULTIBOOT_BOOTLOADER_MAGIC){
    c='a';
   }
   else{
     c='c';
   }
   
  for(int i=0;i<30;i++)terminal_buffer[i] = (uint16_t)c | (uint16_t)(15<<8);
  
}
