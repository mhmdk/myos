#include"filesystem/mbr.h"
#include"filesystem/filesystem.h"
#include"kernel_libc/stdlib.h"
#include"kernel_libc/string.h"
#include"kmalloc.h"
#include"common/stringutils.h"
#include"common/dllist.h"

List* _path_tokens(char *path);
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
List* _path_tokens(char *path) {
	List *tokens = 0;
	while (*path != 0) {
		char *token = kmalloc(12);
		next_token(token, path, '/');
		path+=strlen(token);
		if(*path=='/'){
			path++;
		}
		dllist_insert_at_tail(&tokens, token);
	}
	return tokens;
}

