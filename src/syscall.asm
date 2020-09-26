global syscall

section .text:
syscall:
		mov eax, [esp+4]
		mov ebx, [esp+8]
		int 0x80
		ret
