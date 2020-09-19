#ifndef INCLUDE_PROCESS_H_
#define INCLUDE_PROCESS_H_

#include<stdint.h>
static const uint32_t PROCESS_KERNEL_STACK_SIZE=1024;

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
	char* image_address;
	struct _Process *parent;
	struct TrapFrame *trap_frame;
	Context *context;
} Process;

Process* create_process(uint32_t entry_point);
void clean_process(Process* process);





#endif
