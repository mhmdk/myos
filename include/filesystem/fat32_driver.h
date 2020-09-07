#ifndef INCLUDE_FILESYSTEM_FAT32_DRIVER_H_
#define INCLUDE_FILESYSTEM_FAT32_DRIVER_H_

#include"filesystem/mbr.h"
#include<stdint.h>
#include"common/dllist.h"

// https://en.wikipedia.org/wiki/Design_of_the_FAT_file_system
// https://wiki.osdev.org/FAT32

static const uint32_t BAD_CLUSTER=0x0FFFFFF7;
static const uint32_t FREE_CLUSTER=0x00;

static const uint8_t DELETED_DIRECTORY_ENTRY = 0xE5;
static const uint8_t DOT_DIRECTORY_ENTRY =0x2E;
static const uint8_t LONG_FILENAME_DIRECTORY_ATTRIBUTES =0x0F;

typedef struct {
	// DOS2BiosParameterBlock
	uint16_t bytes_per_sector;
	uint8_t sectors_per_cluster;
	uint16_t reserved_sector_count;
	uint8_t table_count;
	uint16_t root_entry_count;
	uint16_t total_sectors_16;
	uint8_t media_type;
	uint16_t table_size_16;
	uint16_t sectors_per_track;
	// DOS 3.31 BPB:
	uint16_t head_side_count;
	uint32_t hidden_sector_count;
	uint32_t total_sectors_32;

	// FAT32 Extended BIOS Parameter Block:
	uint32_t table_size_32;
	uint16_t extended_flags;
	uint16_t fat_version;
	uint32_t root_cluster;
	uint16_t fat_info;
	uint16_t backup_BS_sector;
	uint8_t reserved_0[12];
	uint8_t drive_number;
	uint8_t reserved_1;
	uint8_t extended_boot_signature; // must be 0x28 or 0x29
	uint32_t volume_id;
	uint8_t volume_label[11];
	uint8_t fat_type_label[8];

} __attribute__((packed)) FAT32BiosParameterBlock;

typedef struct {
	uint8_t jmp_code[3];
	char oem_name[8];
	FAT32BiosParameterBlock bpm;
	uint8_t boot_code[419];
	uint8_t physical_drive_number;
	uint16_t boot_signature;
} __attribute__((packed)) VolumeBootRecord;

typedef struct {
	char name[11];
	uint8_t attributes;
	uint8_t user_attributes;

	char undelete;
	uint16_t create_time;
	uint16_t create_date;
	uint16_t access_date;
	uint16_t cluster_high;

	uint16_t modified_time;
	uint16_t modified_date;
	uint16_t cluster_low;
	uint32_t filesize;
} __attribute__((packed)) DirectoryEntry;

typedef struct {
	PartitionEntry *partition;
	VolumeBootRecord vbr;
} FAT32FileSystem;

typedef struct File_{
char name[128];
struct File_ *parent_directory;
uint32_t  address; // starting cluster
}FAT32File;

FAT32FileSystem* make_fat32filesystem(PartitionEntry *partition);
FAT32File* fat32_get_root_directory(FAT32FileSystem *filesystem);
FAT32File* fat32_open_file(FAT32FileSystem *filesystem, FAT32File *parent_directory,char *name);
List* fat32_list_directory(FAT32FileSystem *filesystem, FAT32File *directory);
int fat32_read_from_file(FAT32FileSystem *filesystem,FAT32File *file, char *buffer,  int number_of_bytes,
		int offset);
int fat32_write_to_file(FAT32FileSystem *filesystem,FAT32File *file, char *buffer,  int number_of_bytes,
		int offset);
int fat32_create_directory(FAT32FileSystem *filesystem,char *name, FAT32File *parent_directory);
int fat32_create_file(FAT32FileSystem *filesystem,char *name, FAT32File *parent_directory);
int fat32_delete_file(FAT32FileSystem *filesystem,FAT32File *file);

#endif
