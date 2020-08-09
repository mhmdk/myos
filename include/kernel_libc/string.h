#ifndef _KERNEL_LIBC_STRING_H_
#define _KERNEL_LIBC_STRING_H_

#include<stdint.h>

void* memset(void *ptr, uint8_t value, uint32_t size);
void* memcpy(void *destination, const  void *source, uint32_t num);
int strlen(char *s);

#endif
