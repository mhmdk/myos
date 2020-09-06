#ifndef INCLUDE_FILESYSTEM_FILESYSTEM_H_
#define INCLUDE_FILESYSTEM_FILESYSTEM_H_

#include"filesystem/fat32_driver.h"
typedef struct {
char label[8];
uint8_t type;
FAT32FileSystem *filesystem;
} Volume;

typedef struct {
FAT32File fat32file;
} File;

Volume *volumes;

void init_filesystem();
File* open_file(char* path);
int read_from_file(File* file, char *buffer,int number_of_blocks,int starting_block);
int write_to_file(File* file, char *buffer,int number_of_blocks,int starting_block);
int create_directory(char* name, File* parent_directory);
int delete_file(File* file);


#endif
