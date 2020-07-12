#include"common/bitmap.h"

BitMap bitmap_wrap_data(uint8_t *data, uint32_t size) {
	BitMap bitmap;
	bitmap.data = data;
	bitmap.size = size;
	return bitmap;
}

uint8_t bitmap_get(BitMap *bitmap, uint32_t index) {
	uint8_t byte_index = index / 8;
	uint8_t bit_index = index % 8;
	return bitmap->data[byte_index] & (1 << bit_index);
}
void bitmap_set(BitMap *bitmap, uint32_t index) {
	uint8_t byte_index = index / 8;
	uint8_t bit_index = index % 8;
	bitmap->data[byte_index] |= (1 << bit_index);
}
void bitmap_unset(BitMap *bitmap, uint32_t index) {
	uint8_t byte_index = index / 8;
	uint8_t bit_index = index % 8;
	//set(just in case) and then flip
	bitmap_set(bitmap,index);
	bitmap->data[byte_index] ^= (1 << bit_index);
}
