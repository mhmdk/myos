#ifndef _PIC_H
#define _PIC_H

#include<stdint.h>

void setup_pic(uint32_t , uint32_t);
void acknowledge_interupt_from_master();
void acknowledge_interupt_from_slave();

#endif
