#include"libc/syscall.h"

int print(char *s){
	return syscall(syscall_print,&s);
}

int sleep(uint32_t milliseconds){
	return syscall(syscall_sleep,&milliseconds);
}
