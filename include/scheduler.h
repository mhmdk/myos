#ifndef INCLUDE_SCHEDULER_H_
#define INCLUDE_SCHEDULER_H_

#include<stdint.h>
#include"process.h"

void init_scheduler(Process* init_process);
void scheduler_add_process(Process* process);
Process* scheduler_current_process();
void schedule();
void yield();
void scheduler_add_process(Process *process);
void ksleep(uint64_t sleep_time);



#endif
