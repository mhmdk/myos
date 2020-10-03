#include "interrupts/idt.h"
#include "interrupts/interrupts.h"

void __fill_trap_gate(IdtEntry*, uint32_t, uint8_t flags);
void __fill_interrupt_gate(IdtEntry*, uint32_t, uint8_t flags);

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
	uint8_t flags = IDT_PRESENT;
	if (interrupt_number == syscall_trap_number) {
		flags |= IDT_PRESENT | IDT_PRIVILEGE_HIGH | IDT_PRIVILEGE_LOW;
		// intel manual vol 3: ch 6.12.1.3 :the only difference between trap and interrupt gate is
		// the way the processor handles the IF flag
		// syscall is a trap gate ,so that interrupts are not disabled
		__fill_trap_gate(&entries[interrupt_number], function, flags);
	} else {
		__fill_interrupt_gate(&entries[interrupt_number], function, flags);
	}
}

void __fill_interrupt_gate(IdtEntry *entry, uint32_t offset, uint8_t flags) {
	fill_idt_entry(entry, offset, interrupt_gate, flags);
}
void __fill_trap_gate(IdtEntry *entry, uint32_t offset, uint8_t flags) {
	fill_idt_entry(entry, offset, trap_gate, flags);
}

