#include "kmalloc.h"
#include"console.h"
#include"kernel_libc/string.h"
#include"common/bitmap.h"

extern uint32_t kmalloc_pool_start;
extern uint32_t _start;

#define CHUNK_SIZE  32
//also defined boot.asm
#define POOL_SIZE 2*MB

static uint8_t allocation_map[POOL_SIZE / (CHUNK_SIZE * 8)];
static uint32_t free_memory = POOL_SIZE;
static uint32_t allocated_memory = 0;
static BitMap bitmap;
uint32_t _first_fit(BitMap *bitmap, const uint32_t number_of_chunks);

void initialize_kmalloc() {
	memset(allocation_map, 0, sizeof(allocation_map));
	bitmap = bitmap_wrap_data(allocation_map, POOL_SIZE / CHUNK_SIZE);
	memset(&kmalloc_pool_start, 0, POOL_SIZE);
}

void* kmalloc(uint32_t size) {
	uint32_t total_size = size + sizeof(AllocationHeader);
	//round up to multiple of chunk size
	uint32_t needed_chunks = (total_size+CHUNK_SIZE-1) / CHUNK_SIZE;
	uint32_t first_chunk_index = _first_fit(&bitmap, needed_chunks);
	if (first_chunk_index >= 0) {
		for (uint32_t i = first_chunk_index; i < first_chunk_index+needed_chunks; i++) {
			bitmap_set(&bitmap, i);
		}
		allocated_memory += total_size;
		free_memory -= total_size;
		AllocationHeader *header =
				(AllocationHeader*) ((uint32_t) (&kmalloc_pool_start)
						+ first_chunk_index * CHUNK_SIZE);
		header->number_of_chunks = needed_chunks;

//		Console console = { 0, 0, SCREEN_BUFFER };
//		print(&console, "allocated: ");
//		print_hex(&console, allocated_memory);
//		print(&console, "\nfree: ");
//		print_hex(&console, free_memory);
//		print(&console, "\nallocated size: ");
//		print_hex(&console, total_size);
//		print(&console, "\nneeded chunks: ");
//		print_hex(&console, needed_chunks);
//		print(&console, "\nfirst chunk: ");
//		print_hex(&console, first_chunk_index);
//		print(&console, "\nheader: ");
//		print_hex(&console, (uint32_t) header);
//		print(&console, "\nheader+1: ");
//		print_hex(&console, (uint32_t) (header + 1));
//		print(&console, "\nsizeof allocatinmap: ");
//		print_hex(&console, sizeof(allocation_map));
//
//		print(&console, "\npool start: ");
//		print_hex(&console, &kmalloc_pool_start);
//		print(&console, "\n start: ");
//		print_hex(&console, (uint32_t)&_start);

		return header + 1;

	} else {
		// TODO panic
		return 0;
	}
}
void kfree(void *ptr) {
	if (!ptr) {
		return;
	}
	AllocationHeader *header = (AllocationHeader*) ((uint8_t*) ptr
			- sizeof(AllocationHeader*));
	uint32_t starting_chunk = ((uint32_t) (header)
			- (uint32_t) &kmalloc_pool_start) / CHUNK_SIZE;
	for (uint32_t chunk = starting_chunk; chunk < header->number_of_chunks;
			chunk++) {
		bitmap_unset(&bitmap, chunk);
	}
	uint32_t total_size = header->number_of_chunks + sizeof(AllocationHeader);
	allocated_memory -= total_size;
	free_memory += total_size;

}

uint32_t _first_fit(BitMap *bitmap, const uint32_t number_of_chunks) {
	uint32_t chunk = 0;
	uint32_t current_consecurive_free = 0;
	while (chunk < bitmap->size) {
		while ((!bitmap_get(bitmap, chunk))
				&& current_consecurive_free < number_of_chunks) {
			current_consecurive_free++;
			chunk++;
		}
		if (current_consecurive_free == number_of_chunks) {
			return chunk - number_of_chunks;
		} else {
			current_consecurive_free = 0;
			chunk++;
		}
	}
	return -1;

}

