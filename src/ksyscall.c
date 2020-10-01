#include"ksyscall.h"
#include"console.h"
#include"scheduler.h"

int ksyscall(uint32_t call_number, uint32_t args[]) {
	switch (call_number) {
	case syscall_exec:
		;
		char *file = (char*) (args[0]);
		return sys_exec(file);
	case syscall_exit:
		;
		uint32_t status = args[0];
		return sys_exit(status);
	case syscall_sleep:
		; //declarations not allowed after label, so we insert an empty statement
		uint32_t time = args[0];
		return sys_sleep(time);
	case syscall_print:
		;
		char *s = (char*) (args[0]);
		return sys_print(s);
	case syscall_getpid:
		return sys_getpid();
	default:
		kprint("unknown sytem call\n");
		kprint_hex(call_number);
		return 0;
	}
}

int sys_print(char *s) {
	kprint(s);
	return 0;
}
int sys_sleep(uint32_t time) {
	ksleep(time);
	return 0;
}

int sys_exit(int status) {
	kexit();
	return 0;
}
int sys_kill(int pid) {
	return kkill(pid);
}
int sys_exec(char *file) {
	return 0;
}

int sys_getpid() {
	return kgetpid();
}
