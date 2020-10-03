#include "process/elf.h"
#include"filesystem/filesystem.h"
#include"memory/kmalloc.h"
#include"drivers/console.h"

void _load_elf_header(Elf32_Ehdr *elf_header, File *elf_file);
void _load_elf_program_headers(Elf32_Phdr *elf_program_headers, File *elf_file,
		int headers_offset, uint32_t number_of_headers);
int _count_loadable_segments(Elf32_Ehdr *elf_header,
		Elf32_Phdr *elf_program_headers);
void _load_segment(char *memory, File *elf_file, Elf32_Phdr *program_header);
uint32_t _load_segments_and_get_entry(Elf32_Ehdr *elf_header,
		Elf32_Phdr *elf_program_headers, char **segments, File *elf_file);

Process* execute_elf(char *elf_file_path) {
	File *elf_file = open_file(elf_file_path);
	if (elf_file == 0) {
		kprint("could not find executable file\n");
		return 0;
	}
	Elf32_Ehdr *elf_header = kmalloc(sizeof(Elf32_Ehdr));
	_load_elf_header(elf_header, elf_file);
	if (((uint32_t*) (elf_header->e_ident))[0] != ELF_MAGIC) {
		kprint("wrong elf magic\n");
		kprint_hex(((uint32_t*) (elf_header->e_ident))[0]);
		return 0;
	}
	if (elf_header->e_phoff == 0) {
		kprint("elf file has no program headers\n");
		return 0;
	}
	Elf32_Phdr *elf_program_headers = kmalloc(
			elf_header->e_phnum * sizeof(Elf32_Phdr));
	_load_elf_program_headers(elf_program_headers, elf_file,
			elf_header->e_phoff, elf_header->e_phnum);

	int number_of_loadable_segments = _count_loadable_segments(elf_header,
			elf_program_headers);
	char **segments = kmalloc(number_of_loadable_segments);
	uint32_t entry = _load_segments_and_get_entry(elf_header,
			elf_program_headers, segments, elf_file);

	kfree(elf_program_headers);
	kfree(elf_header);
	kfree(elf_file);

	Process *process = create_user_process(entry);
	process->image_segments = segments;
	process->number_of_image_segments = number_of_loadable_segments;
	return process;
}

void _load_elf_header(Elf32_Ehdr *elf_header, File *elf_file) {
	read_from_file(elf_file, (char*) elf_header, sizeof(Elf32_Ehdr), 0);
}

void _load_elf_program_headers(Elf32_Phdr *elf_program_headers, File *elf_file,
		int headers_offset, uint32_t number_of_headers) {
	read_from_file(elf_file, (char*) elf_program_headers,
			number_of_headers * sizeof(Elf32_Phdr), headers_offset);
}

void _load_segment(char *memory, File *elf_file, Elf32_Phdr *program_header) {
	if (program_header->p_filesz > 0) {
		read_from_file(elf_file, memory, program_header->p_filesz,
				program_header->p_offset);
	}
	//pad the rest with zeros
	for (int i = program_header->p_filesz; i < program_header->p_memsz; i++) {
		((char*) memory)[i] = 0;
	}
}

int _count_loadable_segments(Elf32_Ehdr *elf_header,
		Elf32_Phdr *elf_program_headers) {
	int segments = 0;
	for (int i = 0; i < elf_header->e_phnum; i++) {
		if (elf_program_headers[i].p_type == ELF_HEADER_TYPE_PT_LOAD) {
			segments++;
		}
	}
	return segments;
}

uint32_t _load_segments_and_get_entry(Elf32_Ehdr *elf_header,
		Elf32_Phdr *elf_program_headers, char **segments, File *elf_file) {
	int segment = 0;
	uint32_t entry = 0;
	for (int i = 0; i < elf_header->e_phnum; i++) {
		if (elf_program_headers[i].p_type == ELF_HEADER_TYPE_PT_LOAD) {
			segments[segment] = (char*) kmalloc(elf_program_headers[i].p_memsz);

//			kprint("loading padress ");
//			kprint_hex((&elf_program_headers[i])->p_paddr);
//			kprint(" into segment address = ");
//			kprint_hex((uint32_t) (segments[segment]));

			_load_segment(segments[segment], elf_file, &elf_program_headers[i]);
			//TODO remove when virtual memory is implemented
			if (elf_header->e_entry >= elf_program_headers[i].p_paddr
					&& elf_header->e_entry
							<= elf_program_headers[i].p_paddr
									+ elf_program_headers[i].p_memsz) {
				entry = elf_header->e_entry - elf_program_headers[i].p_paddr
						+ (uint32_t) segments[segment];
//				kprint("found entry in segment ");
//				kprint_hex(segment);
//				kprint(" address = ");
//				kprint_hex((uint32_t) (segments[segment]));
//				kprint("entry = ");
//				kprint_hex(entry);
//				kprint("\n");
			}
			segment++;
		}
	}
	return entry;
}

