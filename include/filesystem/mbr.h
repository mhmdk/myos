#ifndef INCLUDE_FILESYSTEM_MBR_H_
#define INCLUDE_FILESYSTEM_MBR_H_
#include <stdint.h>

static const int FAT32_PARTITION_TYPE=0x0C;

typedef struct {
	uint8_t bootable; //should be either 0x00 or 0x80
	uint8_t chs_start_address[3];
	uint8_t type;
	uint8_t chs_end_address[3];
	uint32_t lba_frist_sector;
	uint32_t number_of_sectors;
} __attribute__((packed)) PartitionEntry ;

typedef struct {
	// https://en.wikipedia.org/wiki/Master_boot_record
	uint8_t bootstrap_code_1[218];
	uint8_t timestamp[6];
	uint8_t bootstrap_code_2[216];
	uint8_t disk_signature[6];
	PartitionEntry partitions[4];
	uint16_t boot_signature;
}  __attribute__((packed)) Mbr;



void mbr_read();
PartitionEntry* mbr_get_partitions();

#endif
