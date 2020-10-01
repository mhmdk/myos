#include"scheduler.h"
#include"console.h"
#include"common/dllist.h"
#include"kmalloc.h"
#include"interrupts.h"
#include"gdt.h"

void context_switch(Context **old_context, Context *new_context); //defined in asm
void _wakeup(Process *process);
void _sleep(Process *process, uint64_t sleep_time);
void _kill(Process *process);

List *process_list;
Context *scheduler_context;
Process *current_process = 0;

void init_scheduler(Process *init_process) {
	init_process->state = READY;
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
			clean_process(current_process);
			dllist_delete_at_index(&process_list,
					dllist_index_of(process_list, current_process));
		}
		if (current_process->state == SLEEPING
				&& current_process->wake_up_time <= get_time_since_boot()) {
			_wakeup(current_process);
		}
		if (current_process->state == READY) {
			current_process->state = RUNNING;
			set_tss_stack(
					(uint32_t) (current_process->kernel_stack)
							+ PROCESS_KERNEL_STACK_SIZE);
			context_switch(&scheduler_context, current_process->context);
		}

	}
}

void scheduler_add_process(Process *process) {
	process->state = READY;
	dllist_insert_at_tail(&process_list, process);
}

void yield() {
	current_process->state = READY;
	context_switch(&(current_process->context), scheduler_context);
}

void kexit() {
	current_process->state = ZOMBIE;
	context_switch(&(current_process->context), scheduler_context);
}

int kgetpid() {
	return current_process->pid;
}

int kkill(int  pid) {
	struct Node *current_node = process_list;
	Process *process = 0;
	while (current_node != 0) {
		process = (Process*) (current_node->data);
		if (process->pid == pid) {
			break;
		}
	}
	if (process != 0) {
		_kill(process);
	} else {
		return -1;
	}
	return 0;
}
void wakeup(int pid) {
	struct Node *current_node = process_list;
	Process *process = 0;
	while (current_node != 0) {
		process = (Process*) (current_node->data);
		if (process->pid == pid) {
			break;
		}
	}
	if (process != 0) {
		_wakeup(process);
	}
}

void ksleep(uint64_t sleep_time) {
	_sleep(current_process, sleep_time);
	context_switch(&(current_process->context), scheduler_context);
}

void _wakeup(Process *process) {
	process->wake_up_time = 0;
	process->state = READY;
}

void _sleep(Process *process, uint64_t sleep_time) {
	process->wake_up_time = get_time_since_boot() + sleep_time;
	process->state = SLEEPING;
}

void _kill(Process *process){
	process->state = ZOMBIE;
}
