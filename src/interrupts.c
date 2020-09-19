#include"interrupts.h"
#include"pic.h"
#include"console.h"
#include"scheduler.h"
#include"drivers/keyboard.h"

void handle_interrupt(TrapFrame *trap_frame) {
	switch (trap_frame->trap_number) {
	case interrupts_offset:
		print("got time interrupt\n");
		acknowledge_interupt_from_master();
		if (scheduler_current_process()
				&& scheduler_current_process()->state == RUNNING) {
			//disable_interrupts();

			yield();
			//enable_interrupts();
		}

		break;
	case interrupts_offset + 1:
		handle_keyboard_interrupt();
		break;
	case interrupts_offset + 14:
		// ata driver is not interrupt driven, we can ignore
		print("got ata  interrupt  from primary bus\n");
		break;
	case interrupts_offset + 15:
		// ata driver is not interrupt driven, we can ignore
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
void disable_interrupts() {
	__asm__ ("cli");
}
