#ifndef INCLUDE_SCHEDULER_H_
#define INCLUDE_SCHEDULER_H_

#include<stdint.h>
#include"process.h"

void init_scheduler(Process* init_process);
void scheduler_add_process(Process* process);
void schedule();


#endif
