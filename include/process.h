#ifndef INCLUDE_PROCESS_H_
#define INCLUDE_PROCESS_H_

#include<stdint.h>
#include"interrupts.h"

static const uint32_t PROCESS_KERNEL_STACK_SIZE=1024;
static const uint32_t USER_PROCESS_DEFAULT_STACK_SIZE=1024;
static const uint32_t NEW_PROCESS_EFLAGS= 1<<11 ;    // only interrupt flag is set

enum ProcessState {
	 EMBRYO, SLEEPING, READY, RUNNING, ZOMBIE
};

typedef struct {
	// esp is the address of context itself
	uint32_t ebx;
	uint32_t ebp;
	uint32_t edi;
	uint32_t esi;
	uint32_t eip;
} Context;

typedef struct _Process{
	enum ProcessState state;
	int pid;
	char *kernel_stack;
	char* user_stack;
	char* image_address;
	uint32_t image_size;
	struct _Process *parent;
	TrapFrame *trap_frame;
	Context *context;
	uint64_t wake_up_time;
} Process;

Process* create_process(uint32_t entry_point);
void clean_process(Process* process);
Process* create_user_process(uint32_t entry_point);
void clean_user_process(Process* process);





#endif
