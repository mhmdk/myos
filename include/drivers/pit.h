#ifndef INCLUDE_DRIVERS_PIT_H_
#define INCLUDE_DRIVERS_PIT_H_

#include<stdint.h>

// https://wiki.osdev.org/Programmable_Interval_Timer
static const uint32_t TIMER_FREQUENCY = 1193182;
static const uint32_t DEFAULT_PIT_COUNTER=65536;
//TODO make it floating point
static const uint32_t TIME_BETWEEN_TIMER_INTERRUPTS_MS=(DEFAULT_PIT_COUNTER*1000/TIMER_FREQUENCY);
static const uint8_t PIT_CHANNEL_0_PORT=0x40;
static const uint8_t PIT_COMMAND_PORT=0x43;

void init_pit();

#endif
