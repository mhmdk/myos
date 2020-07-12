#include "kernel_libc/string.h"

void* memset(void *ptr, uint8_t value,uint32_t size){
for(uint32_t i =0;i<size;i++){
	((uint8_t*)ptr)[i]=value;
}
return ptr;
}
