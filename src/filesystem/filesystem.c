#include"filesystem/mbr.h"
#include"filesystem/filesystem.h"
#include"kernel_libc/stdlib.h"
#include"kernel_libc/string.h"
#include"kmalloc.h"
#include"common/stringutils.h"
#include"common/dllist.h"

Volume *volumes;
static int number_of_volumes = 0;

List* _path_tokens(char *path);
Volume* _volume_by_label(char *drive_name);
void init_filesystem() {
	mbr_read();

	PartitionEntry *partitions = mbr_get_partitions();
	int number_of_partitions = 0;
	for (int i = 0; i < 4; i++) {
		if (partitions[i].number_of_sectors > 0
				&& partitions[i].type == FAT32_PARTITION_TYPE) {
			number_of_partitions++;
		}
	}
	volumes = (Volume*) kmalloc(number_of_partitions * sizeof(Volume));
	number_of_volumes = number_of_partitions;

	for (int i = 0; i < 4; i++) {
		if (partitions[i].number_of_sectors > 0
				&& partitions[i].type == FAT32_PARTITION_TYPE) {
			strcpy(volumes[i].label, "drv");
			itoa(i, volumes[i].label + 3, 10);
			volumes[i].type = FAT32_PARTITION_TYPE;
			volumes[i].filesystem = make_fat32filesystem(&partitions[i]);
		}
	}
}
File* open_file(char *path) {
	List *tokens = _path_tokens(path);
	struct Node *node = tokens;
	char *drive_name = node->data;
	Volume *volume = _volume_by_label(drive_name);
	if (volume == 0) {
		return 0;
	}
	FAT32File *directory = fat32_get_root_directory(volume->filesystem);
	kprint("root of drive  \n");
	kprint(directory->name);
	kprint("\n");
	kprint_hex(directory->address);
	kprint("\n");
	while (directory != 0 && node->next != 0 && strlen(node->next->data)) {
		kprint("propagating directory :");
		kprint(node->next->data);
		kprint("\n");
		FAT32File *old_directory = directory;
		directory = fat32_open_file(volume->filesystem, directory,
				node->next->data);
		if (node->next != 0 && strlen(node->next->data)) {
			kfree(old_directory);
		}
		node = node->next;
	}
	if (directory == 0) {
		return 0;
	}
	File *file = (File*) kmalloc(sizeof(File));
	file->fat32file = directory;
	file->volume = volume;
	return file;
}

List* list_directory(char *path) {
	File *directory = open_file(path);
	List *entries = fat32_list_directory(directory->volume->filesystem,
			directory->fat32file);
	List *names = 0;
	struct Node *entry = entries;

	while (entry != 0) {
		char *directory_name = ((DirectoryEntry*) entry->data)->name;
		int siize_of_name = sizeof((DirectoryEntry*) entry->data)->name;
		char *name = kmalloc(siize_of_name);
		name[siize_of_name - 1] = 0;
		memcpy(name, directory_name, siize_of_name - 1);
		dllist_insert_at_tail(&names, name);
		entry = entry->next;
	}

	while (entries != 0) {
		struct Node *old_head = entries;
		entries = entries->next;
		kfree(old_head);
	}
	return names;
}

int read_from_file(File *file, char *buffer, int number_of_bytes, int offset) {
	return fat32_read_from_file(file->volume->filesystem, file->fat32file,
			buffer, number_of_bytes, offset);
}
int write_to_file(File *file, char *buffer, int number_of_blocks,
		int starting_block) {
	return 0;
}
int create_directory(char *name, File *parent_directory) {
	return 0;
}
int create_file(char *name, File *parent_directory) {
	return 0;
}
int delete_file(File *file) {
	return 0;
}

List* _path_tokens(char *path) {
	List *tokens = 0;
	while (*path != 0) {
		char *token = kmalloc(12);
		next_token(token, path, '/');
		path += strlen(token);
		if (*path == '/') {
			path++;
		}
		dllist_insert_at_tail(&tokens, token);
	}
	return tokens;
}

Volume* _volume_by_label(char *drive_name) {
	for (int i = 0; i < number_of_volumes; i++) {
		if (!strcmp(drive_name, volumes[i].label)) {
			return &volumes[i];
		}
	}
	return 0;
}

