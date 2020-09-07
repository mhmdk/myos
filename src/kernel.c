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

	FAT32File *root = fat32_get_root_directory(volumes[0].filesystem);
	List *dirlist = fat32_list_directory(volumes[0].filesystem, root);
	dllist_for_each(dirlist, print_directory_entry);
	dllist_free(&dirlist);
	dirlist = 0;

	root = fat32_get_root_directory(volumes[1].filesystem);
	dirlist = fat32_list_directory(volumes[1].filesystem, root);
	dllist_for_each(dirlist, print_directory_entry);
	dllist_free(&dirlist);
	dirlist = 0;

	FAT32File *file = fat32_open_file(volumes[1].filesystem, root, "FILE-P2");
	print("file in partition 2 \n");
	print(file->name);
	print("\n");
	print_hex(file->address);
	print("\n");
//ignore warning , this is a private method and should not be called here
	dirlist = _path_tokens("a/dir1/file1");
	dllist_for_each(dirlist, print);
	dllist_free(&dirlist);
	dirlist = 0;
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

