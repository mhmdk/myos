#ifndef _COMMON_BITMAP_H_
#define _COMMON_BITMAP_H_

#include<stdint.h>
typedef struct{
uint8_t *data;
uint32_t size;
} BitMap;

BitMap bitmap_wrap_data(uint8_t* data,uint32_t size);
BitMap new_bitmap(uint32_t size);
uint8_t bitmap_get(BitMap* bitmap,uint32_t index);
void bitmap_set(BitMap* bitmap,uint32_t index);
void bitmap_unset(BitMap *bitmap, uint32_t index);

#endif
