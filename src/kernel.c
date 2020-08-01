#include<stdint.h>
#include<stddef.h>

#include "kmalloc.h"
#include"drivers/keyboard.h"
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

void print_multiboot_memory_map_entry(Console *console,
		struct multiboot_mmap_entry *entry) {
	print(console, "memory map entry: \n");
//	print(console, "size: ");
//	print_hex(console, entry->size);
//	print(console, "\naddr high: ");
//	print_hex(console, entry->addr >> 32);
	print(console, "\naddr low: ");
	print_hex(console, entry->addr);
//	print(console, "\nlen high:");
//	print_hex(console, entry->len >> 32);
	print(console, "\nlen low:");
	print_hex(console, entry->len);
	print(console, "\ntype: ");
	print_hex(console, entry->type);
	print(console, "\n");
}

void kernel_main(multiboot_uint32_t magic, multiboot_info_t *multibootinfo) {

	GlobalDescriptorTable gdt;
	fill_gdt(&gdt);
	load_gdt(&gdt);

	InterruptDescriptorTable idt;
	fill_idt(&idt);
	load_idt(&idt);

	setup_pic(interrupts_offset, interrupts_offset + 8);
	initialize_kmalloc();
	Console console = new_console();
	set_active_console(&console);
	init_keyboard();
	enable_interrupts();

//	if (magic == MULTIBOOT_BOOTLOADER_MAGIC) {
//		print(&console, "MULTIBOOT_BOOTLOADER_MAGIC is CORRECT\n");
//	} else {
//		print(&console, "MULTIBOOT_BOOTLOADER_MAGIC is INCORRECT\n");
//	}
//	print(&console,"console address: ");
//	print_hex(&console,&console);
//	print(&console,"buffer address: ");
//		print_hex(&console,console.buffer);
//
//	print(&console, "hello\n");
//
//	print(&console, "lower memory size : ");
//	print_hex(&console, multibootinfo->mem_lower);
//	print(&console, "\n");
//
//	print(&console, "upper memory size : ");
//	print_hex(&console, multibootinfo->mem_upper);
//	print(&console, "\n");
//
//	print(&console, "memory map length: ");
//	print_hex(&console, multibootinfo->mmap_length);
//	print(&console, "\n");
//
//	print(&console, "memory map structure address: ");
//	print_hex(&console, multibootinfo->mmap_addr);
//	print(&console, "\n");

	uint32_t offset = 0;
	struct multiboot_mmap_entry *entry;
	while (offset < multibootinfo->mmap_length) {
		entry = (struct multiboot_mmap_entry*) (multibootinfo->mmap_addr
				+ offset);
		//a map of size:entry, so "size" does not include size of itself
		offset += entry->size + sizeof(entry->size);
		//print(&console, "offset = ");
		//print_hex(&console, offset);
		//print(&console, "\n");
		//print_multiboot_memory_map_entry(&console, entry);
	}



	while (1) {
		__asm__ ("hlt");
	}

}


