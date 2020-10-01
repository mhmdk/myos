
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
* project-->properties-->c/c++ general--> pre-processor include paths-->providers tab-->check gcc cross builtin , and replace ${COMMAND} with the cross compiler command and ${FLAGS} with the correct flags ,see https://forum.osdev.org/viewtopic.php?f=13&t=29746

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

## making a partitioned image for testing ##
# the following will make a disk image with dos partition table, and two fat32 formatted partitions

based on https://wiki.osdev.org/Loopback_Device#Partitioning

dd if=/dev/zero of=hda.img bs=512 count=8064 
sudo losetup /dev/loop0 hda.img #may need "modprobe loop" before it
fdisk /dev/loop0
# do fdisk partitioning, pay attension to partition type : FAT32=0x0c
partprobe /dev/loop0 # to tell the kernel about partition table change, now /dev/loop0p1 and p2 should appear
mkdosfs -F32 -v /dev/loop0p1
mkdosfs -F32 -v /dev/loop0p2

or:
sudo losetup  -o 512  --sizelimit 2080256 /dev/loop1 hda.img  # 2080256 = 4063*512
mkdosfs -F32 -v /dev/loop1 #warning  Not enough clusters for a 32 bit FAT! may be emmited
sudo losetup  -o 512  --sizelimit 2080768 /dev/loop2 hda.img  # 2080768 = 4064*512 
mkdosfs -F32 -v /dev/loop2

to use the disk from host machine:
sudo losetup /dev/loop0 hda.img
partprobe /dev/loop0 
# now /dev/loop0p1  /dev/loop0p2 should apear, they can be mounted normally
# to detach device:
sudo losetup -d /dev/loop0 






