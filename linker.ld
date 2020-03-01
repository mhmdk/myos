
ENTRY(_start)

SECTIONS
{
	/* Begin putting sections at 1 MiB, a conventional place for kernels to be loaded at by the bootloader. */
	. = 1M;
 
	/* First put the multiboot header, as it is required to be put very early early in the image or the bootloader won't recognize the file format.*/
	.text BLOCK(4K) :
	{
		*(.multiboot)
		*(.text)
	}
 
	.rodata BLOCK(4K) :
	{
		*(.rodata)
	}
 
	.data BLOCK(4K) :
	{
		*(.data)
	}
 
	/* Read-write data (uninitialized) and stack */
	.bss BLOCK(4K) :
	{
		*(COMMON)
		*(.bss)
	}
 
}
