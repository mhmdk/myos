#include"filesystem/fat32_driver.h"

#include"drivers/ata.h"
#include"kernel_libc/string.h"
#include"kmalloc.h"

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
uint32_t _cluster_of_directory_entry(DirectoryEntry *entry);
void _strip_space_padding(char *stripped_name);
int _bytes_per_cluster(FAT32FileSystem *filesystem) ;

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
	strcpy(file->name, "/");
	file->address = filesystem->vbr.bpm.root_cluster;
	file->size = 0;
	file->attributes = 0;
	return file;
}

List* fat32_list_directory(FAT32FileSystem *filesystem, FAT32File *directory) {
	int directories_per_sector = BYTES_PER_SECTOR / sizeof(DirectoryEntry);
	List *directory_list = 0;
	int current_cluster = directory->address;
	DirectoryEntry *entries = kmalloc(
			directories_per_sector * sizeof(DirectoryEntry));
	int end_of_records = 0;
	while (!end_of_records && _is_data_cluster(current_cluster)) {
		int starting_sector = _first_sector_of_cluster(filesystem,
				current_cluster);
		for (int sector_index = 0;
				sector_index < filesystem->vbr.bpm.sectors_per_cluster
						&& !end_of_records; sector_index++) {
			int sector = filesystem->partition->lba_frist_sector
					+ starting_sector + sector_index;
			ata_read((char*) entries, 0, sector, BYTES_PER_SECTOR);

			for (int i = 0; i < directories_per_sector && !end_of_records;
					i++) {
				if (entries[i].name[0] == 0) {
					end_of_records = 1;
				} else if ((uint8_t) entries[i].name[0]
						!= DELETED_DIRECTORY_ENTRY
						&& entries[i].name[0] != DOT_DIRECTORY_ENTRY
						&& entries[i].attributes
								!= LONG_FILENAME_DIRECTORY_ATTRIBUTES) {
					DirectoryEntry *tmp = (DirectoryEntry*) kmalloc(
							sizeof(DirectoryEntry));
					memcpy(tmp, &entries[i], sizeof(DirectoryEntry));
					dllist_insert_at_tail(&directory_list, tmp);
				}
			}
		}

		current_cluster = _next_cluster(filesystem, current_cluster);
	}

	kfree(entries);
	return directory_list;
}

FAT32File* fat32_open_file(FAT32FileSystem *filesystem,
		FAT32File *parent_directory, char *name) {
	FAT32File *file = 0;

	int directories_per_sector = BYTES_PER_SECTOR / sizeof(DirectoryEntry);
	int current_cluster = parent_directory->address;
	DirectoryEntry *entries = kmalloc(
			directories_per_sector * sizeof(DirectoryEntry));
	int done = 0;
	while (!done && _is_data_cluster(current_cluster)) {

		int starting_sector = _first_sector_of_cluster(filesystem,
				current_cluster);
		for (int sector_index = 0;
				sector_index < filesystem->vbr.bpm.sectors_per_cluster && !done;
				sector_index++) {
			int sector = filesystem->partition->lba_frist_sector
					+ starting_sector + sector_index;
			ata_read((char*) entries, 0, sector, BYTES_PER_SECTOR);

			for (int i = 0; i < directories_per_sector && !done; i++) {
				if (entries[i].name[0] == 0) {
					done = 1;
				} else if ((uint8_t) entries[i].name[0]
						!= DELETED_DIRECTORY_ENTRY
						&& entries[i].name[0] != DOT_DIRECTORY_ENTRY
						&& entries[i].attributes
								!= LONG_FILENAME_DIRECTORY_ATTRIBUTES) {

					char stripped_name[12];
					memcpy(stripped_name, entries[i].name, 11);
					_strip_space_padding(stripped_name);
					if (!strcmp(stripped_name, name)) {
						file = (FAT32File*) kmalloc(sizeof(FAT32File));
						file->address = _cluster_of_directory_entry(
								&entries[i]);
						file->parent_directory = parent_directory;
						strcpy(file->name, name);

						file->size = entries[i].filesize;
						file->attributes = entries[i].attributes;
						done = 1;
					}
				}
			}
		}

		current_cluster = _next_cluster(filesystem, current_cluster);
	}
	kfree(entries);
	return file;
}

int fat32_read_from_file(FAT32FileSystem *filesystem, FAT32File *file,
		char *buffer, int number_of_bytes, int offset) {

	if (number_of_bytes > file->size + offset) {
		number_of_bytes = file->size;
	}

	int cluster_offset = offset / _bytes_per_cluster(filesystem);
	int byte_offset = offset % _bytes_per_cluster(filesystem);
	int current_cluster = file->address + cluster_offset;

	int read_bytes = 0;
	char *tmp = kmalloc(BYTES_PER_SECTOR);
	while (_is_data_cluster(current_cluster) && read_bytes < number_of_bytes) {
		int starting_sector = _first_sector_of_cluster(filesystem,
				current_cluster);
		for (int sector_index = 0;
				sector_index < filesystem->vbr.bpm.sectors_per_cluster
						&& read_bytes < number_of_bytes; sector_index++) {
			int sector = filesystem->partition->lba_frist_sector
					+ starting_sector + sector_index;
			int bytes_to_read = number_of_bytes - read_bytes;
			if (bytes_to_read > BYTES_PER_SECTOR) {
				bytes_to_read = BYTES_PER_SECTOR;
			}
			if (bytes_to_read > BYTES_PER_SECTOR - byte_offset) {
				bytes_to_read = BYTES_PER_SECTOR - byte_offset;
			}

			ata_read(tmp, 0, sector, BYTES_PER_SECTOR);
			memcpy(buffer + read_bytes, tmp + byte_offset, bytes_to_read);
			read_bytes += bytes_to_read;
			byte_offset=0;
		}

		current_cluster = _next_cluster(filesystem, current_cluster);
	}
	kfree(tmp);

	return read_bytes;
}


int _find_free_cluster(FAT32FileSystem *filesystem) {
	int current_sector = _table_first_sector(filesystem);
	int fat_last_sector = current_sector + _table_number_of_sectors(filesystem)
			- 1;
	uint32_t entries_per_sector = BYTES_PER_SECTOR / sizeof(uint32_t);
	uint32_t *clusters = (uint32_t*) kmalloc(BYTES_PER_SECTOR);

	while (current_sector <= fat_last_sector) {
		ata_read((char*)clusters, 0, current_sector, BYTES_PER_SECTOR);
		for (int cluster_index = 0; cluster_index < entries_per_sector;
				cluster_index++) {
			if (clusters[cluster_index] == 0) {
				kfree(clusters);
				return (current_sector - _table_first_sector(filesystem))
						* entries_per_sector + cluster_index;
			}
		}
	}
	kfree(clusters);
	return -1;
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
	//relative to beginning of partition
	int fat_sector = _table_first_sector(filesystem)
			+ ((cluster * 4) / BYTES_PER_SECTOR);
	uint32_t fat_offset = (cluster * 4) % 512;
	_read_into_cache(filesystem, fat_sector);
	uint32_t next_cluster = *((uint32_t*) (&cache.buffer[fat_offset]))
			& 0x0FFFFFFF;
	return next_cluster;

}

void _read_into_cache(FAT32FileSystem *filesystem, int sector) {
	int absolute_sector = filesystem->partition->lba_frist_sector + sector;
	if (cache.sector_number != absolute_sector) {
		ata_read((char*) cache.buffer, 0, absolute_sector, BYTES_PER_SECTOR);
		cache.sector_number = absolute_sector;
	}
}

int _is_end_of_chain(int next_cluster) {
	return (next_cluster & 0x0FFFFFFF) >= 8;
}

int _is_data_cluster(int next_cluster) {
	return (next_cluster & 0x0FFFFFFF) > 1
			&& (next_cluster & 0x0FFFFFFF) < 0x0FFFFFF0;
}
int _bytes_per_cluster(FAT32FileSystem *filesystem) {
	return filesystem->vbr.bpm.sectors_per_cluster * BYTES_PER_SECTOR;
}

uint32_t _cluster_of_directory_entry(DirectoryEntry *entry) {
	return (uint32_t) entry->cluster_low
			| ((uint32_t) entry->cluster_high) << 16;
}
void _strip_space_padding(char *stripped_name) {
	for (int i = 10; i > 0 && stripped_name[i] == ' '; i--) {
		stripped_name[i] = 0;
	}
}

