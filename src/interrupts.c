#include"interrupts.h"
#include"pic.h"
#include"console.h"

//one disadvantage of writing each ISR separately is that we need to know
//where it is mapped to decide whether to acknowledge_from_slave or not


__attribute__((interrupt)) void dummy_isr(void *unused) {
	Console console={3,0};
	print(&console,"got timer interrupt  \n");

	__asm__("nop");
	acknowledge_interupt_from_master();
}

__attribute__((interrupt)) void isr_01_keyboard(void *unused) {
	//TODO remove when we have a memory manager
	//another console object , will clash with the one in kernel
	Console console={4,0};
	print(&console,"got keyboard input \n");
	//acknowledge only from master because keyboard interrupt (IRQ offset+1) comes from master
	acknowledge_interupt_from_master();
}

void enable_interrupts() {
	__asm__ ("sti");
}
