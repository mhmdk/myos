#include"process/process.h"
#include"memory/kmalloc.h"
#include"memory/gdt.h"

extern uint32_t trap_return;
int next_pid = 1;
Process* create_process(uint32_t entry_point) {
	Process *process = (Process*) (kmalloc(sizeof(Process)));
	process->pid = next_pid++;
	process->state = EMBRYO;
	process->parent = 0;
	process->trap_frame = 0;
	process->wake_up_time = 0;
	process->number_of_image_segments = 0;
	process->kernel_stack = (char*) kmalloc(PROCESS_KERNEL_STACK_SIZE);
	process->user_stack = 0;

	process->context = (Context*) (process->kernel_stack
			+ PROCESS_KERNEL_STACK_SIZE - sizeof(Context));
	process->context->eip = entry_point;
	process->context->ebp = (uint32_t) process->kernel_stack;
	process->context->esi = 0;
	process->context->edi = 0;
	process->context->ebx = 0;

	return process;
}
void clean_process(Process *process) {
	for (int i = 0; i < process->number_of_image_segments; i++) {
		kfree(process->image_segments[i]);
	}
	if (process->user_stack != 0) {
		kfree(process->user_stack);
	}
	kfree(process->kernel_stack);
	kfree(process);
}

Process* create_user_process(uint32_t entry_point) {
	Process *process = (Process*) (kmalloc(sizeof(Process)));
	process->pid = next_pid++;
	process->state = EMBRYO;
	process->parent = 0;
	process->wake_up_time = 0;
	process->number_of_image_segments = 0;
	process->kernel_stack = (char*) kmalloc(PROCESS_KERNEL_STACK_SIZE);

	process->user_stack = kmalloc(USER_PROCESS_DEFAULT_STACK_SIZE);

	process->trap_frame = (TrapFrame*) (process->kernel_stack
			+ PROCESS_KERNEL_STACK_SIZE - sizeof(TrapFrame));

	process->trap_frame->eip = entry_point;
	process->trap_frame->cs = usermode_code_segment_selector;

	process->trap_frame->ebp = (uint32_t) process->user_stack
			+ USER_PROCESS_DEFAULT_STACK_SIZE;
	process->trap_frame->esp = process->trap_frame->ebp;
	process->trap_frame->ss = usermode_data_segment_selector;

	process->trap_frame->eflags = NEW_PROCESS_EFLAGS;

	process->trap_frame->ds = usermode_data_segment_selector;
	process->trap_frame->gs = usermode_data_segment_selector;
	process->trap_frame->fs = usermode_data_segment_selector;
	process->trap_frame->es = usermode_data_segment_selector;

	process->trap_frame->edi = 0;
	process->trap_frame->esi = 0;
	process->trap_frame->old_esp = 0;
	process->trap_frame->edx = 0;
	process->trap_frame->ecx = 0;
	process->trap_frame->eax = 0;

	process->context = (Context*) ((uint32_t) (process->trap_frame)
			- sizeof(Context));
	process->context->eip = (uint32_t) (&trap_return);
	process->context->ebp = (uint32_t) process->kernel_stack
			+ PROCESS_KERNEL_STACK_SIZE;
	process->context->esi = 0;
	process->context->edi = 0;
	process->context->ebx = 0;

	return process;
}
