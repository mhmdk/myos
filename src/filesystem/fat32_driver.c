#include"filesystem/fat32_driver.h"
#include"filesystem/filesystem.h"
#include"drivers/ata.h"
#include"kernel_libc/string.h"
#include"kmalloc.h"

int _read_cluster(FAT32FileSystem *filesystem, int cluster, uint8_t *buffer);
int _write_cluster(FAT32FileSystem *filesystem, int cluster, uint8_t *buffer);
int _table_first_sector(FAT32FileSystem *filesystem);
int _is_end_of_chain(int next_cluster);
int _is_data_cluster(int next_cluster);
int _total_number_of_sectors(FAT32FileSystem *filesystem);
int _table_number_of_sectors(FAT32FileSystem *filesystem);
int _first_data_sector(FAT32FileSystem *filesystem);
int _total_data_sectors(FAT32FileSystem *filesystem);
int _total_clusters(FAT32FileSystem *filesystem);
int _first_sector_of_cluster(FAT32FileSystem *filesystem, int cluster);
uint32_t _next_cluster(FAT32FileSystem *filesystem, int cluster);
void _read_into_cache(FAT32FileSystem *filesystem, int sector);
int _is_end_of_chain(int next_cluster);
int _is_data_cluster(int next_cluster);

FAT32FileSystem* make_fat32filesystem(PartitionEntry *partition) {
	FAT32FileSystem *filesystem = (FAT32FileSystem*) kmalloc(
			sizeof(FAT32FileSystem));
	// read boot record of each partition
	ata_read((char*) (&filesystem->vbr), 0, partition->lba_frist_sector,
			BYTES_PER_SECTOR);
	filesystem->partition = partition;
	return filesystem;
}
typedef struct {
	uint8_t buffer[512];
	int sector_number;
} Cache;

Cache cache;

FAT32File* fat32_get_root_directory(FAT32FileSystem *filesystem) {
	FAT32File *file = (FAT32File*) kmalloc(sizeof(FAT32File));
	file->parent_directory = 0;
	strcpy(file->path, "/");
	file->address = filesystem->vbr.bpm.root_cluster;
	return file;
}

List* fat32_list_directory(FAT32FileSystem *filesystem, FAT32File *directory) {
	int directories_per_sector = BYTES_PER_SECTOR / sizeof(DirectoryEntry);
	DirectoryEntry *entries = kmalloc(directories_per_sector);
	List *directory_list = 0;
	int starting_sector = _first_sector_of_cluster(filesystem,
			directory->address);
	ata_read((char*) entries, 0,
			filesystem->partition->lba_frist_sector + starting_sector, 512);

	for (int i = 0; i < directories_per_sector && entries[i].name[0] != 0;
			i++) {
		if ((uint8_t) entries[i].name[0] != DELETED_DIRECTORY_ENTRY
				&& entries[i].name[0] != DOT_DIRECTORY_ENTRY) {
			DirectoryEntry *tmp = (DirectoryEntry*) kmalloc(
					sizeof(DirectoryEntry));
			memcpy(tmp, &entries[i], sizeof(DirectoryEntry));
			dllist_insert_at_tail(&directory_list, tmp);
		}
	}
	kfree(entries);
	return directory_list;
}

FAT32File* fat32_open_file(FAT32FileSystem *filesystem, char *path) {
	return 0;
}
int fat32_read_from_file(FAT32File *file, char *buffer, int number_of_bytes,
		int offset) {
	return 0;
}
int fat32_write_to_file(FAT32File *file, char *buffer, int number_of_bytes,
		int offset) {
	return 0;
}
int fat32_create_directory(char *name, FAT32File *parent_directory) {
	return 0;
}
int fat32_delete_file(FAT32File *file) {
	return 0;
}

int _read_cluster(FAT32FileSystem *filesystem, int cluster, uint8_t *buffer) {
	return 0;
}
int _write_cluster(FAT32FileSystem *filesystem, int cluster, uint8_t *buffer) {
	return 0;
}

int _table_first_sector(FAT32FileSystem *filesystem) {
	return filesystem->vbr.bpm.reserved_sector_count;
}

int _total_number_of_sectors(FAT32FileSystem *filesystem) {
	return filesystem->vbr.bpm.total_sectors_16 == 0 ?
			filesystem->vbr.bpm.total_sectors_32 :
			filesystem->vbr.bpm.total_sectors_16;
}

int _table_number_of_sectors(FAT32FileSystem *filesystem) {
	return filesystem->vbr.bpm.table_size_16 == 0 ?
			filesystem->vbr.bpm.table_size_32 :
			filesystem->vbr.bpm.table_size_16;
}
int _first_data_sector(FAT32FileSystem *filesystem) {
	return filesystem->vbr.bpm.reserved_sector_count
			+ _table_number_of_sectors(filesystem)
					* filesystem->vbr.bpm.table_count;
}

int _total_data_sectors(FAT32FileSystem *filesystem) {
	return _total_number_of_sectors(filesystem)
			- filesystem->vbr.bpm.reserved_sector_count
			- _table_number_of_sectors(filesystem)
					* filesystem->vbr.bpm.table_count;
}
int _total_clusters(FAT32FileSystem *filesystem) {
	return _total_data_sectors(filesystem)
			/ filesystem->vbr.bpm.sectors_per_cluster;
}

int _first_sector_of_cluster(FAT32FileSystem *filesystem, int cluster) {
	return _first_data_sector(filesystem)
			+ ((cluster - 2) * filesystem->vbr.bpm.sectors_per_cluster);
}

uint32_t _next_cluster(FAT32FileSystem *filesystem, int cluster) {
	if (cluster > _total_clusters(filesystem)) {
		return BAD_CLUSTER;
	}
	int fat_sector = (uint32_t) filesystem->partition->lba_frist_sector
			+ _table_first_sector(filesystem)
			+ ((cluster * 4) / BYTES_PER_SECTOR);
	uint32_t fat_offset = (cluster * 4) % 512;
	_read_into_cache(filesystem, fat_sector);
	uint32_t next_cluster = *((uint32_t*) (&cache.buffer[fat_offset]))
			& 0x0FFFFFFF;
	return next_cluster;

}

void _read_into_cache(FAT32FileSystem *filesystem, int sector) {
	if (cache.sector_number != sector) {
		ata_read((char*) cache.buffer, 0,
				filesystem->partition->lba_frist_sector + sector,
				BYTES_PER_SECTOR);
		cache.sector_number = sector;
	}
}

int _is_end_of_chain(int next_cluster) {
	return (next_cluster & 0x0FFFFFFF) >= 8;
}

int _is_data_cluster(int next_cluster) {
	return (next_cluster & 0x0FFFFFFF) > 1
			&& (next_cluster & 0x0FFFFFFF) < 0x0FFFFFF0;
}

