extern kernel_main
global _start

	;multiboot specification: https://www.gnu.org/software/grub/manual/multiboot/multiboot.html
MEM_BOUNDARY_ALIGN  equ  1 << 0            
PROVIDE_MEM_INFO  equ  1 << 1            
FLAGS    equ  MEM_BOUNDARY_ALIGN | PROVIDE_MEM_INFO 
MAGIC    equ  0x1BADB002        
CHECKSUM equ -(MAGIC + FLAGS)   

	
section .multiboot 		;multiboot structure
align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM


section .bss
align 16			;stack on x86 must be 16-byte aligned according to the System V ABI standard
stack_bottom: 			;trick to use kernel bss as stack, this should probably be changed
	resb 16*1024
stack_top:

section .text
_start:
	mov esp,stack_top
	push ebx		;multiboot boot information
	push eax 		;multiboot magic

	call kernel_main

	cli 			; should not be reached
	hlt			; lock the device 
	
	
	
	
