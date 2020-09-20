#include"scheduler.h"
#include"console.h"
#include"process.h"
#include"common/dllist.h"
#include"kmalloc.h"
#include"interrupts.h"
#include"gdt.h"

void context_switch(Context **old_context, Context *new_context);

List *process_list;
Context *scheduler_context;
Process *current_process = 0;

void init_scheduler(Process *init_process) {
	process_list = new_dllist(init_process);
}

Process* scheduler_current_process() {
	return current_process;
}

void schedule() {
	struct Node *current_node = process_list;
	load_tss();
	while (1) {
		// per intel manual v3, par 6.8 :When an interrupt is handled through an interrupt gate
		//and when the timer interrupt calls yield , the timer ISR in the context of previous process is not finished yet
		//so we have to enable interrupts again, otherwise the next process will never be interrupted
		enable_interrupts();

		if (current_node == 0) {
			current_node = process_list;
		}
		current_process = current_node->data;
		current_node = current_node->next;

		if (current_process->state == ZOMBIE) {
			dllist_delete_at_index(&process_list,
					dllist_index_of(process_list, current_process));
		} else if (current_process->state == READY) {
			current_process->state = RUNNING;
			set_tss_stack(
					(uint32_t) (current_process->kernel_stack)
							+ PROCESS_KERNEL_STACK_SIZE);
			context_switch(&scheduler_context, current_process->context);
		}

	}
}

void yield() {
	current_process->state = READY;
	context_switch(&(current_process->context), scheduler_context);
}
void scheduler_add_process(Process *process) {
	process->state = READY;
	dllist_insert_at_tail(&process_list, process);
}

