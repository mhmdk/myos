#include"process.h"


int next_pid = 1;
Process* create_process(uint32_t entry_point) {
	Process *process = (Process*) (kmalloc(sizeof(Process)));
	process->pid = next_pid++;
	process->state = READY;
	process->image_address = 0;
	process->parent = 0;
	process->trap_frame = 0;
	process->kernel_stack = (char*)kmalloc(PROCESS_KERNEL_STACK_SIZE);

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
	kfree(process->kernel_stack);
	kfree(process);
}
