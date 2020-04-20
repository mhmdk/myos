on manjaro:
intalled  xorriso and mtools for grub-mkrescue

to debug:
qemu-system-x86_64 -machine q35 -kernel kernel.elf -s -S

gdb kernel.elf
set arch i386:x86-64
target remote localhost:1234 