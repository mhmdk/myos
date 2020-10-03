#ifndef _PORTS_H
#define _PORTS_H

#include<stdint.h>

//TODO inl, inw , outl, outw
void outb(uint16_t port, uint8_t val);
void outw(uint16_t port, uint16_t val);
uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);

#endif
