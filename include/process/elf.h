#ifndef INCLUDE_PROCESS_ELF_H_
#define INCLUDE_PROCESS_ELF_H_

#include <stdint.h>
#include"process/process.h"

// https://docs.oracle.com/cd/E23824_01/html/819-0690/chapter7-6.html
typedef uint16_t Elf32_Half;
typedef uint32_t Elf32_Word;
typedef uint32_t Elf32_Addr;
typedef uint32_t Elf32_Off;
typedef int32_t Elf32_Sword;

static const uint32_t ELF_MAGIC = 0x464C457F; //"\x7fELF";

//https://docs.oracle.com/cd/E23824_01/html/819-0690/chapter6-43405.html

#define EI_NIDENT       16
typedef struct {
	unsigned char e_ident[EI_NIDENT];
	Elf32_Half e_type;
	Elf32_Half e_machine;
	Elf32_Word e_version;
	Elf32_Addr e_entry;
	Elf32_Off e_phoff;
	Elf32_Off e_shoff;
	Elf32_Word e_flags;
	Elf32_Half e_ehsize;
	Elf32_Half e_phentsize;
	Elf32_Half e_phnum;
	Elf32_Half e_shentsize;
	Elf32_Half e_shnum;
	Elf32_Half e_shstrndx;
} Elf32_Ehdr;

static const Elf32_Half ELF_FILE_TYPE_ET_EXEC = 2;

// https://docs.oracle.com/cd/E23824_01/html/819-0690/chapter6-83432.html
typedef struct {
	Elf32_Word p_type;
	Elf32_Off p_offset;
	Elf32_Addr p_vaddr;
	Elf32_Addr p_paddr;
	Elf32_Word p_filesz;
	Elf32_Word p_memsz;
	Elf32_Word p_flags;
	Elf32_Word p_align;
} Elf32_Phdr;

static const uint32_t ELF_HEADER_TYPE_PT_LOAD = 1;

Process* execute_elf( char *elf_file_path);

#endif
