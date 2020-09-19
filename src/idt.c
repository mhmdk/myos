#include"idt.h"
#include"interrupts.h"

//void __fill_trap_gate(IdtEntry*,uint32_t );
void __fill_interrupt_gate(IdtEntry*, uint32_t);
void __fill_unused_gate(IdtEntry*);
//void __fill_tss_gate(IdtEntry*);
void setup_all_interrupts();

IdtEntry entries[IDT_SIZE];

void load_idt(InterruptDescriptorTable *idt) {
	asm("lidt (%0)"::"p"(idt));
}

void fill_idt(InterruptDescriptorTable *idt) {
	setup_all_interrupts();
	idt->adderess = entries;
	//size = 8*N -1 where 8 is the size of one descriptor
	idt->size = sizeof(IdtEntry) * IDT_SIZE - 1;
}

void set_interrupt_handler(uint8_t interrupt_number, uint32_t function) {
	__fill_interrupt_gate(&entries[interrupt_number], function);
}

void __fill_interrupt_gate(IdtEntry *entry, uint32_t offset) {
	fill_idt_entry(entry, offset, interrupt_gate, IDT_PRESENT);
}
void __fill_unused_gate(IdtEntry *entry) {
	fill_idt_entry(entry, 0, interrupt_gate, 0);
}
