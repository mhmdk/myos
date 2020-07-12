#ifndef   _MEMORY_MANAGEMENT_H
#define _MEMORY_MANAGEMENT_H

#include<stdint.h>


typedef uint32_t address;

#define KB  1024
#define MB  1024 * KB

typedef struct {
	uint32_t number_of_chunks;
} AllocationHeader;

void* kmalloc(uint32_t size);
void kfree(void*);
void initialize_kmalloc();

#endif
