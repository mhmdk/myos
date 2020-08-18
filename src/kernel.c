#include<stdint.h>
#include<stddef.h>

#include "kmalloc.h"
#include"keyboard_layout.h"
#include"drivers/keyboard.h"
#include"drivers/vga.h"
#include "multiboot.h"
#include "gdt.h"
#include "idt.h"
#include "interrupts.h"
#include "pic.h"
#include"console.h"
#include"terminal.h"

#if defined(__linux__)
#error "compiling for linux"
#endif 

#if !defined(__i386__)
#error "not i386"
#endif

void print_multiboot_memory_map_entry(struct multiboot_mmap_entry *entry) {
	print("memory map entry: \n");
//	print(console, "size: ");
//	print_hex(console, entry->size);
//	print(console, "\naddr high: ");
//	print_hex(console, entry->addr >> 32);
	print("\naddr low: ");
	print_hex(entry->addr);
//	print(console, "\nlen high:");
//	print_hex(console, entry->len >> 32);
	print("\nlen low:");
	print_hex(entry->len);
	print("\ntype: ");
	print_hex(entry->type);
	print("\n");
}

void print_memory_map(multiboot_info_t *multibootinfo) {
	print("lower memory size : ");
	print_hex(multibootinfo->mem_lower);
	print("\n");
	print("upper memory size : ");
	print_hex(multibootinfo->mem_upper);
	print("\n");
	print("memory map length: ");
	print_hex(multibootinfo->mmap_length);
	print("\n");
	print("memory map structure address: ");
	print_hex(multibootinfo->mmap_addr);
	print("\n");
	uint32_t offset = 0;
	struct multiboot_mmap_entry *entry;
	while (offset < multibootinfo->mmap_length) {
		entry = (struct multiboot_mmap_entry*) (multibootinfo->mmap_addr
				+ offset);
		//a map of size:entry, so "size" does not include size of itself
		offset += entry->size + sizeof(entry->size);
		print("offset = ");
		print_hex(offset);
		print("\n");
		print_multiboot_memory_map_entry(entry);
	}
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
	init_keyboard();
	init_vga();
	init_console();
	enable_interrupts();

	if (magic == MULTIBOOT_BOOTLOADER_MAGIC) {
		print("MULTIBOOT_BOOTLOADER_MAGIC is CORRECT\n");
	} else {
		print("MULTIBOOT_BOOTLOADER_MAGIC is INCORRECT\n");
	}
	print_memory_map(multibootinfo);

	terminal_main();

	while (1) {
		__asm__ ("hlt");
	}
}



