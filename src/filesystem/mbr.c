#include"filesystem/mbr.h"
#include "drivers/ata.h"
#include"console.h"
#include"kmalloc.h"

Mbr *mbr;
void mbr_read() {
	mbr = (Mbr*) kmalloc(sizeof(Mbr));
	ata_read((char*)mbr, 0, 0, BYTES_PER_SECTOR);
	if (mbr->boot_signature != 0xAA55) {
		//not mbr
		print("incorrect signature\n");
		print_hex(mbr->boot_signature);
	} else {
		for (int i = 0; i < 4; i++) {
			if ((mbr->partitions[i]).type == 0x0c) {
				print("FAT32 partition detected\n");
				print("partition start: ");
				print_hex((mbr->partitions[i]).lba_frist_sector);
				print("\npartition size: ");
				print_hex((mbr->partitions[i]).number_of_sectors);
				print("\n");
			} else {
				print("unknown partition type\n");
			}
		}
	}
}

PartitionEntry* mbr_get_partitions(){
return mbr->partitions;
}

