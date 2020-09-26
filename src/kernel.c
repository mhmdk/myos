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
#include"scheduler.h"
#include"drivers/pit.h"
#include"user_mode_test.h"

#if defined(__linux__)
#error "compiling for linux"
#endif 

#if !defined(__i386__)
#error "not i386"
#endif

void print_multiboot_memory_map_entry(struct multiboot_mmap_entry *entry) {
	kprint("memory map entry: \n");
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

void test_ata_driver() {
	char *data = "0123456789\n";
	int write_count = ata_write(data, 0, 20, 11);
	kprint("written");
	kprint_hex(write_count);
	kprint("bytes\n");
	char *buffer = (char*) kmalloc(20);
	buffer[12] = 0;
	int read_count = ata_read(buffer, 0, 20, 11);
	kprint("read ");
	kprint_hex(read_count);
	kprint("bytes\n");
	kprint(buffer);
}

void print_directory_entry(void *arg) {
	DirectoryEntry *entry = (DirectoryEntry*) arg;
	entry->name[11] = 0;
	kprint(entry->name);
	kprint("\n");
	kprint_hex(entry->attributes);
	kprint("\n");
}


void kernel_main(multiboot_uint32_t magic, multiboot_info_t *multibootinfo) {

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
	//init_filesystem();
	init_pit();

	//Process *processB = create_process((uint32_t) taskB);
	//Process *processA = create_process((uint32_t) taskA);
	Process *processB = create_user_process((uint32_t) taskB);
	Process *processA = create_user_process((uint32_t) taskA);
	init_scheduler(processA);
	scheduler_add_process(processB);
	schedule();

	File *filep2 = open_file("drv1/FILE-P2");
	kprint("file in drive 2 \n");
	kprint(filep2->fat32file->name);
	kprint("\n");
	kprint_hex(filep2->fat32file->address);
	kprint("\n");

	File *filep1 = open_file("drv0/FILE-P1");
	kprint("file in drive 1\n");
	kprint(filep1->fat32file->name);
	kprint("\n");
	kprint_hex(filep1->fat32file->address);
	kprint("\n");

	char *buffer = kmalloc(1024);
	kprint("reading from file p1\n");
	read_from_file(filep1, buffer, 20, 0);
	kprint(buffer);
	kprint("reading from file p2\n");
	read_from_file(filep2, buffer, 20, 0);
	kprint(buffer);

	List *directories = list_directory("drv0/");
	dllist_for_each(directories, kprint);

	directories = list_directory("drv0/DIR1/");
	dllist_for_each(directories, kprint);
	kfree(directories);

	File *filemcf = open_file("drv0/MCS");
	kprint("file in drive0/mcs\n");
	kprint(filemcf->fat32file->name);
	kprint("\n");
	kprint_hex(filemcf->fat32file->address);
	kprint("\n");
	kprint_hex(filemcf->fat32file->size);
	kprint("\n");
	kprint("reading from multi cluster file\n");
	read_from_file(filemcf, buffer, 1024, 0);
	kprint(buffer);
//test_ata_driver();
	if (magic == MULTIBOOT_BOOTLOADER_MAGIC) {
		kprint("MULTIBOOT_BOOTLOADER_MAGIC is CORRECT\n");
	} else {
		kprint("MULTIBOOT_BOOTLOADER_MAGIC is INCORRECT\n");
	}
	//print_memory_map(multibootinfo);
	terminal_main();
	while (1) {
		__asm__ ("hlt");
	}
}

