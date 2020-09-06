#ifndef _KERNEL_LIBC_STRING_H_
#define _KERNEL_LIBC_STRING_H_

#include<stdint.h>

void* memset(void *ptr, uint8_t value, uint32_t size);
void* memcpy(void *destination, const  void *source, uint32_t num);
// void* memmove(void *destination, const  void *source, uint32_t num);
int strlen(const char *s);
int strcmp(const char *s1, const char*s2);
int strspn(const char *str, const char *seps);
const char * strchr ( const char * str, int character );
char* strcpy(char* destination, const char* source);

#endif
