#include"ksyscall.h"
#include"console.h"
#include"scheduler.h"


int ksyscall(uint32_t call_number, uint32_t args[]){
	switch (call_number){
	case syscall_exec:
		return 0;
	case syscall_exit:
		return 0;
	case syscall_sleep:
		; //declarations not allowed after label, so we insert an empty statement
		uint32_t time = args[0];
		return sys_sleep(time);
	case syscall_print:
		;
		char *s = (char*)(args[0]);
		return sys_print(s);
	default:
		kprint("unknown sytem call\n");
		kprint_hex(call_number);
		return 0;
	}
}

int sys_print(char* s){
	 kprint(s);
	 return 0;
}
int sys_sleep(uint32_t time){
	ksleep(time);
	return 0;
}
