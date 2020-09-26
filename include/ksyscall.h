#ifndef INCLUDE_KSYSCALL_H_
#define INCLUDE_KSYSCALL_H_

#include<stdint.h>

#define syscall_exec 1
#define syscall_exit  2
#define  syscall_sleep  3
#define  syscall_print  4

int ksyscall(uint32_t number, uint32_t* args);

int sys_print(char* s);
int sys_sleep(uint32_t time);
int sys_exit(int status);
int sys_exec(char* file);

#endif
