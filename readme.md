
## on manjaro: ##
intalled  xorriso and mtools for grub-mkrescue

## run from command line: ##
qemu-system-x86_64 -machine pc -kernel kernel.elf [-hda hda.img]
where hda.img is a disk image file 

## debugging from command line: ##
qemu-system-x86_64 -machine pc -kernel kernel.elf -s -S [-hda hda.img]

gdb kernel.elf
set arch i386:x86-64
target remote localhost:1234 

## in eclipse: ##

### 1.  fix compiler errors ###
* project-->properties-->c/c++ build--> untick "generate makefile automatically ; and specify build location
* project-->properties-->c/c++ build--> pre-processor include paths-->providers tab-->check gcc cross builtin , and replace ${COMMAND} with the cross compiler command and ${FLAGS} with the correct flags ,see https://forum.osdev.org/viewtopic.php?f=13&t=29746

### 2.  add run configuration: ###
*  **qemu** : run-->external tools--> add qemu command
*  **qemu-debug**: duplicate the qemu run configuration and add -s -S to arguments
*  **debug** : 
	1. create a gdbinit file in the projects directory , and add "set arch i386:x86-64" to it
	2. debug-->debug configuration-->c/c++ remote application specify kernel.elf in program path
	3. at the bottom: click "select other"-->manual remote debugging launcher
	4. in debugger: select the gdbinit created for this project
	5. in debugger: connection tab --> specify the correct port qemu is listening to

## making an iso ##
see makeiso.sh
