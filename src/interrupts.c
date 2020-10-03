#include"interrupts.h"
#include"pic.h"
#include"console.h"
#include"scheduler.h"
#include"drivers/keyboard.h"
#include"drivers/pit.h"
#include"ksyscall.h"

uint64_t time_since_boot = 0;

void panic(TrapFrame *trap_frame);

void handle_interrupt(TrapFrame *trap_frame) {
	//print("got an interrupt\n");
	switch (trap_frame->trap_number) {
	case interrupts_offset:
		//kprint("got time interrupt\n");
		time_since_boot += TIME_BETWEEN_TIMER_INTERRUPTS_MS;
		acknowledge_interupt_from_master();
		if (scheduler_current_process()->state == RUNNING) {
			yield();
		} else if (scheduler_current_process()->state == ZOMBIE) {
			// example : in case command :kill {terminal_pid}  is issued
			kexit();
		} else {
			kprint(
					"timer interrupt occured and current process is in an illegal state\n");
			panic(trap_frame);
		}

		break;
	case interrupts_offset + 1:
		handle_keyboard_interrupt();
		break;
	case interrupts_offset + 14:
		// ata driver is not interrupt driven, we can ignore
		kprint("got ata  interrupt  from primary bus\n");
		break;
	case interrupts_offset + 15:
		// ata driver is not interrupt driven, we can ignore
		kprint("got ata  interrupt  from secondary bus\n");
		break;
	case syscall_trap_number:
		//kprint("syscall\n");
		trap_frame->eax = ksyscall(trap_frame->eax,
				(uint32_t*) trap_frame->ebx);
		break;
	default:
		kprint("unknown interrupt\n");
		kprint_hex(trap_frame->trap_number);
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

uint64_t get_time_since_boot() {
	return time_since_boot;
}

void panic(TrapFrame *trap_frame) {
	kprint("PANIC!!\n");

	kprint("\n edi = ");
	kprint_hex(trap_frame->edi);
	kprint("\n esi = ");
	kprint_hex(trap_frame->esi);
	kprint("\n ebp = ");
	kprint_hex(trap_frame->ebp);
	kprint("\n edi = ");
	kprint_hex(trap_frame->edi);
	kprint("\n old_esp = ");
	kprint_hex(trap_frame->old_esp);
	kprint("\n ecx = ");
	kprint_hex(trap_frame->ecx);
	kprint("\n eax = ");
	kprint_hex(trap_frame->eax);
	kprint("\n gs = ");
	kprint_hex(trap_frame->gs);
	kprint("\n fs = ");
	kprint_hex(trap_frame->fs);
	kprint("\n es = ");
	kprint_hex(trap_frame->es);
	kprint("\n ds = ");
	kprint_hex(trap_frame->ds);
	kprint("\n eip = ");
	kprint_hex(trap_frame->eip);
	kprint("\n cs = ");
	kprint_hex(trap_frame->cs);
	kprint("\n eflags = ");
	kprint_hex(trap_frame->eflags);
	kprint("\n esp = ");
	kprint_hex(trap_frame->esp);
	kprint("\n ss = ");
	kprint_hex(trap_frame->ss);

	asm("hlt");
}
