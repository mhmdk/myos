#include<stdint.h>
#include<stddef.h>
#include "multiboot.h"
#include "gdt.h"
#include "idt.h"
#include "interrupts.h"
#include "pic.h"
#include"console.h"

#if defined(__linux__)
#error "compiling for linux"
#endif 

#if !defined(__i386__)
#error "not i386"
#endif


void kernel_main(multiboot_uint32_t magic, multiboot_info_t *multibootinfo) {

	GlobalDescriptorTable gdt;
	fill_gdt(&gdt);
	load_gdt(&gdt);

	InterruptDescriptorTable idt;
	fill_idt(&idt);
	load_idt(&idt);

	setup_pic(interrupts_offset, interrupts_offset + 8);
	enable_interrupts();

	Console console = { 0, 0 };
	if (magic == MULTIBOOT_BOOTLOADER_MAGIC) {
		print(&console, "MULTIBOOT_BOOTLOADER_MAGIC is CORRECT\n");
	} else {
		print(&console, "MULTIBOOT_BOOTLOADER_MAGIC is INCORRECT\n");
	}
	print(&console, "hello\n");

	while (1) {
		__asm__ ("hlt");
	}

}
