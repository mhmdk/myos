#include"interrupts.h"
#include"pic.h"
#include"console.h"

#include"drivers/keyboard.h"
//one disadvantage of writing each ISR separately is that we need to know
//where it is mapped to decide whether to acknowledge_from_slave or not

__attribute__((interrupt)) void dummy_isr(void *unused) {
	//print( "got timer interrupt  \n");

	__asm__("nop");
	acknowledge_interupt_from_master();
}

__attribute__((interrupt)) void isr_01_keyboard(void *unused) {

	handle_keyboard_interrupt();

	//acknowledge only from master because keyboard interrupt (IRQ offset+1) comes from master
	acknowledge_interupt_from_master();
}
__attribute__((interrupt)) void isr_14_ata(void* unused){
	print( "got ata  interrupt  from primary bus\n");
	acknowledge_interupt_from_slave();
}
__attribute__((interrupt)) void isr_15_ata(void* unused){
	print( "got ata  interrupt  from secondary bus\n");
	acknowledge_interupt_from_slave();
}
void enable_interrupts() {
	__asm__ ("sti");
}
