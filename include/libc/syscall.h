#ifndef INCLUDE_LIBC_SYSCALL_H_
#define INCLUDE_LIBC_SYSCALL_H_
#include<stdint.h>

static const uint32_t syscall_exec= 1;
static const uint32_t syscall_exit = 2;
static const uint32_t syscall_sleep = 3;
static const uint32_t syscall_print = 4;

int syscall(uint32_t number, void *args);

int print(char *s);
int sleep(uint32_t milliseconds);
#endif
