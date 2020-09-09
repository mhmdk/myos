#include<stdint.h>
#include<stddef.h>

#include "kmalloc.h"
#include"keyboard_layout.h"
#include"drivers/keyboard.h"
#include"drivers/vga.h"
#include"drivers/ata.h"
#include "multiboot.h"
#include "gdt.h"
#include "idt.h"
#include "interrupts.h"
#include "pic.h"
#include"console.h"
#include "filesystem/filesystem.h"
#include"terminal.h"
#include"common/dllist.h"

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

void test_ata_driver() {
	char *data = "0123456789\n";
	int write_count = ata_write(data, 0, 20, 11);
	print("written");
	print_hex(write_count);
	print("bytes\n");
	char *buffer = (char*) kmalloc(20);
	buffer[12] = 0;
	int read_count = ata_read(buffer, 0, 20, 11);
	print("read ");
	print_hex(read_count);
	print("bytes\n");
	print(buffer);
}

void print_directory_entry(void *arg) {
	DirectoryEntry *entry = (DirectoryEntry*) arg;
	entry->name[11] = 0;
	print(entry->name);
	print("\n");
	print_hex(entry->attributes);
	print("\n");
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
	ata_detect();
	enable_interrupts();

	init_filesystem();


	File *filep2 = open_file("drv1/FILE-P2");
	print("file in drive 2 \n");
	print(filep2->fat32file->name);
	print("\n");
	print_hex(filep2->fat32file->address);
	print("\n");

	File *filep1 = open_file("drv0/FILE-P1");
	print("file in drive 1\n");
	print(filep1->fat32file->name);
	print("\n");
	print_hex(filep1->fat32file->address);
	print("\n");

	char *buffer = kmalloc(1024);
	print("reading from file p1\n");
	read_from_file(filep1, buffer, 20, 0);
	print(buffer);
	print("reading from file p2\n");
	read_from_file(filep2, buffer, 20, 0);
	print(buffer);

	List *directories = list_directory("drv0/");
	dllist_for_each(directories, print);

	directories = list_directory("drv0/DIR1/");
	dllist_for_each(directories, print);
	kfree(directories);

	File *filemcf = open_file("drv0/MCS");
	print("file in drive0/mcs\n");
	print(filemcf->fat32file->name);
	print("\n");
	print_hex(filemcf->fat32file->address);
	print("\n");
	print_hex(filemcf->fat32file->size);
	print("\n");
	print("reading from multi cluster file\n");
	read_from_file(filemcf, buffer, 1024, 0);
	print(buffer);
//test_ata_driver();
	if (magic == MULTIBOOT_BOOTLOADER_MAGIC) {
		print("MULTIBOOT_BOOTLOADER_MAGIC is CORRECT\n");
	} else {
		print("MULTIBOOT_BOOTLOADER_MAGIC is INCORRECT\n");
	}
	//print_memory_map(multibootinfo);

	terminal_main();

	while (1) {
		__asm__ ("hlt");
	}
}

