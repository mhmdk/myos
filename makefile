
#based on https://wiki.osdev.org/Makefile

CFLAGS := -I include -g -std=gnu99 -ffreestanding -nostdlib -O2 -Wall -mgeneral-regs-only  #-Wextra

# -mgeneral-regs-only to avoid error in ISR functions, https://forum.osdev.org/viewtopic.php?f=1&t=32455


CC := $(HOME)/opt/cross/bin/i686-elf-gcc

PROJDIRS := $(shell pwd)
TARGET := target
DEPS_TARGET := dependencies
# find files as list of relative paths
SRCFILES := $(shell find $(PROJDIRS) -type f -name "*.c" -printf '%P ')
ASMFILES := $(shell find $(PROJDIRS) -type f -name "*.asm" -printf '%P ')
HDRFILES := $(shell find $(PROJDIRS) -type f -name "*.h" -printf '%P ')
OBJFILES := $(patsubst src/%.c,$(TARGET)/%.o,$(SRCFILES)) $(patsubst src/%.asm,$(TARGET)/%.o,$(ASMFILES))
DEPFILES := $(patsubst %.o,%.d,$(OBJFILES))

.PHONY: clean run test compile install all

-include $(DEPFILES)

all: clean install
	@echo all

clean:
	@echo cleaning
	rm -rf $(TARGET)

run:
	@echo running
	qemu-system-x86_64 -machine q35 -kernel kernel.elf &

test:
	@echo test #TODO

install: compile
	@echo installing
	cp $(TARGET)/kernel.elf . 

compile: $(TARGET)/kernel.elf
	@echo compiling

$(TARGET)/kernel.elf: $(OBJFILES) linker.ld makefile
	$(CC) $(CFLAGS) -T linker.ld -lgcc -o $@ $(OBJFILES) 

$(TARGET)/%.o: src/%.c makefile | $(TARGET)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(TARGET)/%.o: src/%.asm makefile | $(TARGET)
	nasm -felf32 $< -o $@

$(TARGET):
	if [[ ! -d $(TARGET) ]] ; then mkdir $(TARGET) ; fi

