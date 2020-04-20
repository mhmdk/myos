#include"interrupts.h"
#include"pic.h"

__attribute__((interrupt)) void dummy_isr(void* unused){

 static uint16_t* const terminal_buffer=(uint16_t*) 0xB8000;

 char c='t';
 for(int i=0;i<300;i++)terminal_buffer[i] = (uint16_t)c | (uint16_t)(15<<8);

 __asm__("nop");
 acknowledge_interupt_from_master();
  }


__attribute__((interrupt)) void isr_01_keyboard(void* unused){

  static uint16_t* const terminal_buffer=(uint16_t*) 0xB8000;

 char c='k';
 for(int i=0;i<400;i++)terminal_buffer[i] = (uint16_t)c | (uint16_t)(15<<8);
 acknowledge_interupt_from_master();
}

void enable_interrupts(){
  __asm__ ("sti");
}
