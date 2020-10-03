#include"memory/gdt.h"
#include"kernel_libc/string.h"

TaskStateSegment task_state_segment;

void __fill_null_selector(GdtEntry*);
void __fill_code_selector(GdtEntry*);
void __fill_data_selector(GdtEntry*);
void __fill_usermode_code_selector(GdtEntry*);
void __fill_usermode_data_selector(GdtEntry*);
void __fill_tss_selector(GdtEntry*);
void __reload_data_segment_registers();

void load_gdt(GlobalDescriptorTable *gdt) {
	__asm__("lgdt (%0)"::"p"(gdt));

	//this far call (previously long jump )will reload the CS register
	__asm__("call %0,%1"::"n"(code_segment_selector),"p"(__reload_data_segment_registers));

	// because long call pushes both segment descriptor and eip,
	// but the ret in __reload_data_segment_registers pops only eip
	asm("add $4,%ESP");
}

void fill_gdt(GlobalDescriptorTable *gdt) {

	__fill_null_selector(&(gdt->null_segment));
	__fill_code_selector(&(gdt->code_segment));
	__fill_data_selector(&(gdt->data_segment));
	__fill_tss_selector(&(gdt->tss_segment));
	__fill_usermode_code_selector(&(gdt->usermode_code_segment));
	__fill_usermode_data_selector(&(gdt->usermode_data_segment));
	__fill_null_selector(&(gdt->extra_segment));

	gdt->gdt_descriptor.adderess = &(gdt->null_segment);
	//size = 8*N -1 where 8 is the size of one descriptor
	gdt->gdt_descriptor.size = sizeof(GlobalDescriptorTable)
			- sizeof(GDTDescriptor) - 1;
}

void load_tss(){
__asm__("ltr %0\n"::"r"(tss_segment_selector) );
}

void set_tss_stack(uint32_t esp0){
	task_state_segment.esp0=esp0;
	task_state_segment.ss0=data_segment_selector;
}

void __fill_null_selector(GdtEntry *entry) {
	fill_gdt_entry(entry, 0, 0, 0);
}

void __fill_code_selector(GdtEntry *entry) {
	uint8_t flags = GDT_PRESENT | CODE_OR_DATA | EXECUTABLE
			| READABLE_OR_WRITABLE;
	fill_gdt_entry(entry, 0, 0xffffffff, flags);
}

void __fill_data_selector(GdtEntry *entry) {
	uint8_t flags = GDT_PRESENT | CODE_OR_DATA | READABLE_OR_WRITABLE;
	fill_gdt_entry(entry, 0, 0xfffffff, flags);
}
void __fill_usermode_code_selector(GdtEntry *entry) {
	uint8_t flags = GDT_PRESENT | CODE_OR_DATA | EXECUTABLE
			| READABLE_OR_WRITABLE | GDT_PRIVILEGE_HIGH | GDT_PRIVILEGE_LOW;
	fill_gdt_entry(entry, 0, 0xffffffff, flags);
}

void __fill_usermode_data_selector(GdtEntry *entry) {
	uint8_t flags = GDT_PRESENT | CODE_OR_DATA | READABLE_OR_WRITABLE
			| GDT_PRIVILEGE_HIGH | GDT_PRIVILEGE_LOW;
	fill_gdt_entry(entry, 0, 0xfffffff, flags);
}

void __fill_tss_selector(GdtEntry *entry) {
	fill_gdt_tss_entry(entry, (uint32_t) (&task_state_segment),
			sizeof(TaskStateSegment));

	memset(&task_state_segment, 0, sizeof(task_state_segment));
}

void __reload_data_segment_registers() {
	//per multiboot specifications those segments have offset 0 originally
	//so we are not really changing anything
	//otherwise we have to deal with a messed up stack??

	__asm__("push %%EAX\n\t"
			"mov %0,%%AX\n\t"
			"mov %%AX,%%DS\n\t"
			"mov %%AX,%%ES\n\t"
			"mov %%AX,%%FS\n\t"
			"mov %%AX,%%GS\n\t"
			"mov %%AX,%%SS\n\t"
			"pop %%EAX"::"n"(data_segment_selector));
}
