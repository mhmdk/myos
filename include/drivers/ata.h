#ifndef INCLUDE_DRIVERS_ATA_H_
#define INCLUDE_DRIVERS_ATA_H_

#include <stdint.h>

static const uint16_t PRIMARY_BUS_IO_PORT_BASE = 0x1F0;
static const uint16_t SECONDARY_BUS_IO_PORT_BASE = 0x170;

static const uint16_t DATA_REGISTER_OFFSET = 0;
static const uint16_t ERROR_REGISTER_OFFSET = 1;
static const uint16_t FEATURES_REGISTER_OFFSET = 1;
static const uint16_t SECTOR_COUNT_REGISTER_OFFSET = 2;
static const uint16_t LBA_LOW_REGISTER_OFFSET = 3;
static const uint16_t LBA_MID_REGISTER_OFFSET = 4;
static const uint16_t LBA_HIGH_REGISTER_OFFSET = 5;
static const uint16_t DRIVE_REGISTER_OFFSET = 6;
static const uint16_t STATUS_REGISTER_OFFSET = 7;
static const uint16_t COMMAND_REGISTER_OFFSET = 7;
static const uint16_t CONTROL_REGISTER_OFFSET = 0x206;

static const uint8_t DRIVE_REGISTER_SLAVE_FLAG = 1 << 4;
static const uint8_t DRIVE_REGISTER_LBA_FLAG = 1 << 6;

static const uint8_t STATUS_REGISTER_ERR_FLAG = 1;
static const uint8_t STATUS_REGISTER_DRQ_FLAG = 1 << 3;
static const uint8_t STATUS_REGISTER_DF_FLAG=1<<5;
static const uint8_t STATUS_REGISTER_BSY_FLAG = 1 << 7;

static const uint8_t CONTROL_REGISTER_NOINTERRUPT_FLAG = 1 << 1;

static const uint8_t IDENTIFY_COMMAND = 0xEC;
static const uint8_t READ_NATIVE_MAX_ADDRESS_COMMAND = 0xF8;
static const uint8_t READ_SECTORS_COMMAND = 0x20;
static const uint8_t WRITE_SECTORS_COMMAND = 0x30;
static const uint8_t CACHE_FLUSH_COMMAND=0xE7;

static const uint32_t BYTES_PER_SECTOR = 512;

typedef struct {
	int master;
	int primary;
	uint16_t io_base_port;
	int max_size;
	int lb48;
	int device_id; //custom for our os, currently only one ata device with id zero
} ata_drive;

int ata_read(char *buffer, int device_id, uint32_t starting_sector, uint32_t byte_count);
int ata_write(char *buffer, int device_id, uint32_t starting_sector, uint32_t byte_count);
void ata_detect();

#endif
