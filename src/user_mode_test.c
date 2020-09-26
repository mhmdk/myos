#include "user_mode_test.h"
#include "libc/syscall.h"

void taskA() {
	while (1) {
		print("A");
		sleep(1000);
	}
}

void taskB() {
	while (1) {
		print("B");
		sleep(100);
	}
}


