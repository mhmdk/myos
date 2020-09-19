#include"interrupts.h"
#include"pic.h"
#include"console.h"

#include"drivers/keyboard.h"
//one disadvantage of writing each ISR separately is that we need to know
//where it is mapped to decide whether to acknowledge_from_slave or not

void handle_interrupt(TrapFrame *trap_frame) {
	switch (trap_frame->trap_number) {
	case interrupts_offset:
		//print("got time interrupt\n");
		break;
	case interrupts_offset + 1:
		handle_keyboard_interrupt();
		break;
	case interrupts_offset + 14:
		print("got ata  interrupt  from primary bus\n");
		break;
	case interrupts_offset + 15:
		print("got ata  interrupt  from secondary bus\n");
		break;
	case syscall_trap_number:
		print("syscall\n");
		break;
	default:
		print("unknown interrupt\n");
		print_hex(trap_frame->trap_number);
	}

	if (trap_frame->trap_number >= interrupts_offset
			&& trap_frame->trap_number
					< interrupts_offset + number_of_interrupts_per_pic) {
		acknowledge_interupt_from_master();
	}
	if (trap_frame->trap_number
			>= interrupts_offset + number_of_interrupts_per_pic
			&& trap_frame->trap_number
					< interrupts_offset + 2 * number_of_interrupts_per_pic) {
		acknowledge_interupt_from_slave();
	}
}
void enable_interrupts() {
	__asm__ ("sti");
}
