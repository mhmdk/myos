#!/bin/bash

mkdir -p isodir/boot/grub
echo "menuentry \"myos\" {
	multiboot /boot/kernel.elf
}" > isodir/boot/grub/grub.cfg

cp kernel.elf isodir/boot/
grub-mkrescue -o os.iso isodir
