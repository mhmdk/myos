#include"filesystem/mbr.h"
#include "drivers/ata.h"
#include"drivers/console.h"
#include"memory/kmalloc.h"


Mbr *mbr;
void mbr_read() {
	mbr = (Mbr*) kmalloc(sizeof(Mbr));
	ata_read((char*)mbr, 0, 0, BYTES_PER_SECTOR);
	if (mbr->boot_signature != 0xAA55) {
		//not mbr
		kprint("incorrect MBR signature\n");
		kprint_hex(mbr->boot_signature);
	} else {
		for (int i = 0; i < 4; i++) {
			if ((mbr->partitions[i]).type == 0x0c) {
				kprint("FAT32 partition detected\n");
				kprint("partition starts at sector ");
				kprint_hex((mbr->partitions[i]).lba_frist_sector);
				kprint("\npartition size is ");
				kprint_hex((mbr->partitions[i]).number_of_sectors);
				kprint(" sectors\n");
			} else {
				//kprint("unknown partition type\n");
			}
		}
	}
}

PartitionEntry* mbr_get_partitions(){
return mbr->partitions;
}

