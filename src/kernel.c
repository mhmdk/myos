#include<stdint.h>
#include<stddef.h>

#include "interrupts/idt.h"
#include "interrupts/interrupts.h"
#include "interrupts/interrupts.h"
#include "interrupts/pic.h"
#include "multiboot.h"
#include "memory/kmalloc.h"
#include"drivers/keyboard.h"
#include"drivers/vga.h"
#include"drivers/pit.h"
#include"drivers/ata.h"
#include "filesystem/filesystem.h"
#include"process/scheduler.h"
#include"process/elf.h"
#include "memory/gdt.h"
#include"drivers/console.h"
#include"terminal.h"
#include"common/dllist.h"


#if defined(__linux__)
#error "compiling for linux"
#endif 

#if !defined(__i386__)
#error "not i386"
#endif

void print_memory_map(multiboot_info_t *multibootinfo);

void kernel_main(multiboot_uint32_t magic, multiboot_info_t *multibootinfo) {

	disable_interrupts();
	GlobalDescriptorTable gdt;
	fill_gdt(&gdt);
	load_gdt(&gdt);

	InterruptDescriptorTable idt;
	fill_idt(&idt);
	load_idt(&idt);

	setup_pic(interrupts_offset,
	interrupts_offset + number_of_interrupts_per_pic - 1);
	initialize_kmalloc();
	init_keyboard();
	init_vga();
	init_console();
	ata_detect();
	init_filesystem();
	init_pit();

	if (magic == MULTIBOOT_BOOTLOADER_MAGIC) {
		kprint("MULTIBOOT_BOOTLOADER_MAGIC is CORRECT\n");
	} else {
		kprint("MULTIBOOT_BOOTLOADER_MAGIC is INCORRECT\n");
	}
	print_memory_map(multibootinfo);

	Process *process_terminal = create_process((uint32_t) terminal_main);
	init_scheduler(process_terminal);
	schedule();

	while (1) {
		__asm__ ("hlt");
	}
}

void print_multiboot_memory_map_entry(struct multiboot_mmap_entry *entry) {
	kprint("\nmemory map entry:");
//	print(console, "size: ");
//	print_hex(console, entry->size);
//	print(console, "\naddr high: ");
//	print_hex(console, entry->addr >> 32);
	kprint("\naddr low: ");
	kprint_hex(entry->addr);
//	print(console, "\nlen high:");
//	print_hex(console, entry->len >> 32);
	kprint("\nlen low:");
	kprint_hex(entry->len);
	kprint("\ntype: ");
	kprint_hex(entry->type);
	kprint("\n");
}

void print_memory_map(multiboot_info_t *multibootinfo) {
	kprint("memory map: \n");
	kprint("lower memory size : ");
	kprint_hex(multibootinfo->mem_lower);
	kprint("\n");
	kprint("upper memory size : ");
	kprint_hex(multibootinfo->mem_upper);
	kprint("\n");
	kprint("memory map length: ");
	kprint_hex(multibootinfo->mmap_length);
	kprint("\n");
	kprint("memory map structure address: ");
	kprint_hex(multibootinfo->mmap_addr);
	kprint("\n");
	uint32_t offset = 0;
	struct multiboot_mmap_entry *entry;
	while (offset < multibootinfo->mmap_length) {
		entry = (struct multiboot_mmap_entry*) (multibootinfo->mmap_addr
				+ offset);
		//a map of size:entry, so "size" does not include size of itself
		offset += entry->size + sizeof(entry->size);
		kprint("offset = ");
		kprint_hex(offset);
		kprint("\n");
		print_multiboot_memory_map_entry(entry);
	}
}

