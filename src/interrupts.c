#include"interrupts.h"
#include"pic.h"
#include"console.h"
#include"scheduler.h"
#include"drivers/keyboard.h"
#include"drivers/pit.h"

uint64_t time_since_boot=0;


void panic(TrapFrame* trap_frame);

void handle_interrupt(TrapFrame *trap_frame) {
	//print("got an interrupt\n");
	switch (trap_frame->trap_number) {
	case interrupts_offset:
		//print("got time interrupt\n");
		time_since_boot+=TIME_BETWEEN_TIMER_INTERRUPTS_MS;
		acknowledge_interupt_from_master();
		if (scheduler_current_process()
				&& scheduler_current_process()->state == RUNNING) {
		yield();
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
		panic(trap_frame);
	}

	// > and not >=interrupts_offset because the timer interrupt is acknowledged already before jumping to scheduler
	if (trap_frame->trap_number > interrupts_offset
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

uint64_t get_time_since_boot(){
	return time_since_boot;
}

void panic(TrapFrame* trap_frame){
print("PANIC!!\n");

print("\n edi = ");
print_hex(trap_frame->edi);
print("\n esi = ");
print_hex(trap_frame->esi);
print("\n ebp = ");
print_hex(trap_frame->ebp);
print("\n edi = ");
print_hex(trap_frame->edi);
print("\n old_esp = ");
print_hex(trap_frame->old_esp);
print("\n ecx = ");
print_hex(trap_frame->ecx);
print("\n eax = ");
print_hex(trap_frame->eax);
print("\n gs = ");
print_hex(trap_frame->gs);
print("\n fs = ");
print_hex(trap_frame->fs);
print("\n es = ");
print_hex(trap_frame->es);
print("\n ds = ");
print_hex(trap_frame->ds);
print("\n eip = ");
print_hex(trap_frame->eip);
print("\n cs = ");
print_hex(trap_frame->cs);
print("\n eflags = ");
print_hex(trap_frame->eflags);
print("\n esp = ");
print_hex(trap_frame->esp);
print("\n ss = ");
print_hex(trap_frame->ss);

asm("hlt");
}
