#include"drivers/pit.h"
#include"ports.h"

void _set_pit_reload_counter(uint32_t counter);

void init_pit(){
	_set_pit_reload_counter(DEFAULT_PIT_COUNTER);
}

void _set_pit_reload_counter(uint32_t counter){
	if(counter>0xFFFF){
		counter =0 ; // 0 means 65536
	}
//0x34 = 00110100b        channel 0, lobyte/hibyte, rate generator
	uint8_t pit_mode=0x34;
	outb(PIT_COMMAND_PORT,pit_mode);
	uint8_t counter_low_byte =counter&0xFF;
	uint8_t counter_high_byte =(counter>>8)&0xFF;
	outb(PIT_CHANNEL_0_PORT,counter_low_byte);
	outb(PIT_CHANNEL_0_PORT,counter_high_byte);
}

