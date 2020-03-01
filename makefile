
CFLAGS := -g -std=gnu99 -ffreestanding -nostdlib -O2 -Wall #-Wextra
CC := $(HOME)/opt/cross/bin/i686-elf-gcc

PROJDIRS := $(shell pwd)
TARGET := target
SRCFILES := $(shell find $(PROJDIRS) -type f -name "*.c")
ASMFILES := $(shell find $(PROJDIRS) -type f -name "*.asm")
HDRFILES := $(shell find $(PROJDIRS) -type f -name "*.h")
OBJFILES := $(patsubst %.c,%.o,$(SRCFILES)) $(patsubst %.asm,%.o,$(ASMFILES))
DEPFILES := $(patsubst %.c,%.d,$(SRCFILES))

.PHONY: clean run test compile install 

-include $(DEPFILES)

clean:
	@echo cleaning
	rm -rf target

run:
	@echo running
	qemu-system-x86_64 -machine q35 -kernel kernel.elf &

test:
	@echo test #TODO

install:
	@echo installing

compile: kernel.elf
	@echo compiling

kernel.elf: $(OBJFILES) linker.ld makefile | $(TARGET)
	$(CC) $(CFLAGS) -T linker.ld -lgcc -o $@ $(OBJFILES) 

%.o: %.c makefile
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

%.o: %.asm makefile
	nasm -felf32 $< -o $@

$(TARGET):
	if [[ ! -d $(TARGET)]]; do
		mkdir $(TARGET)
	done
