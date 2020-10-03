#include"drivers/ata.h"
#include"drivers/ports.h"
#include"drivers/console.h"
#include"memory/kmalloc.h"

// http://www.t13.org/documents/UploadedDocuments/project/d1153r18-ATA-ATAPI-4.pdf
// see parallel ata page on wikipedia for other standard versions links

ata_drive *main_drive = 0;

int _read_from_drive(char *buffer, ata_drive*, uint32_t starting_sector,
		uint32_t byte_count);
int _write_to_drive(char *buffer, ata_drive*, uint32_t starting_sector,
		uint32_t byte_count);
int _identify(int primary, int slave);
int _read_size(ata_drive*);
int _read_size_lb28(ata_drive *drive);
void _read_identify_output(ata_drive *drive);
void _wait_for_device(uint16_t io_base_port);
int _poll(uint16_t io_base_port);
void _issue_ata_command(uint16_t io_base_port, uint16_t command);
void _select_lba28_address(uint16_t io_base_port, int slave,
		uint32_t starting_sector);
void _cache_flush(ata_drive *drive);

void ata_detect() {
	int drive_found = _identify(0, 0);
	if (!drive_found) {
		drive_found = _identify(1, 0);
	}
	if (!drive_found) {
		drive_found = _identify(0, 1);
	}
	if (!drive_found) {
		drive_found = _identify(1, 1);
	}
}

int ata_read(char *buffer, int device_id, uint32_t starting_sector,
		uint32_t byte_count) {
	//device id to select drive, but we have only one drive
	return _read_from_drive(buffer, main_drive, starting_sector, byte_count);

}
int ata_write(char *buffer, int device_id, uint32_t starting_sector,
		uint32_t byte_count) {
	//device id to select drive, but we have only one drive
	return _write_to_drive(buffer, main_drive, starting_sector, byte_count);
}

int _identify(int primary, int slave) {
	uint16_t io_base_port =
			primary ? PRIMARY_BUS_IO_PORT_BASE : SECONDARY_BUS_IO_PORT_BASE;

	outb(io_base_port + CONTROL_REGISTER_OFFSET,
			0 | CONTROL_REGISTER_NOINTERRUPT_FLAG);	//disable irqs

	uint8_t status_register = inb(io_base_port + STATUS_REGISTER_OFFSET);
	if (status_register == 0xFF) {
		return 0;	//bus has no drives
	}
	_select_lba28_address(io_base_port, slave, 0);
	outb(io_base_port + SECTOR_COUNT_REGISTER_OFFSET, 0);
	_issue_ata_command(io_base_port, IDENTIFY_COMMAND);

	status_register = inb(io_base_port + STATUS_REGISTER_OFFSET);
	if (status_register == 0) {
		return 0;	//drive does not exist
	}
	while (status_register & STATUS_REGISTER_BSY_FLAG) {
		status_register = inb(io_base_port + STATUS_REGISTER_OFFSET);
	}

	uint8_t lba_mid_register = inb(io_base_port + LBA_MID_REGISTER_OFFSET);
	uint8_t lba_high_register = inb(io_base_port + LBA_HIGH_REGISTER_OFFSET);
	if (lba_mid_register || lba_high_register) {
		return 0;	//not ATA
	}

	while (!(status_register & STATUS_REGISTER_DRQ_FLAG)
			&& !(status_register & STATUS_REGISTER_ERR_FLAG)) {
		status_register = inb(io_base_port + STATUS_REGISTER_OFFSET);
	}
	if (status_register & STATUS_REGISTER_ERR_FLAG) {
		kprint(
				"ERROR: ATA drive detected but ERR flag of status register set\n");
		//the error can  cleared by sending a new command to the drive
		//but we will exit
		return 0;
	}

	main_drive = (ata_drive*) (kmalloc(sizeof(ata_drive)));
	main_drive->device_id = 0;
	main_drive->master = slave ? 0 : 1;
	main_drive->primary = primary;
	main_drive->io_base_port = io_base_port;
	_read_identify_output(main_drive);
	_read_size(main_drive);

	kprint("info: ATA drive ");
	kprint(main_drive->master ? "master" : "slave");
	kprint(primary ? " on primary bus" : " on secondary bus");
	kprint(" detected \n");
	kprint("drive size = ");
	kprint_hex(main_drive->max_size);
	kprint(" sectors\n");

	return 1;
}

int _read_from_drive(char *buffer, ata_drive *drive, uint32_t starting_sector,
		uint32_t byte_count) {
	uint32_t sector_count = (byte_count + BYTES_PER_SECTOR-1) / BYTES_PER_SECTOR;
	if (starting_sector + sector_count >= drive->max_size) {
		return -1;
	}
	_select_lba28_address(drive->io_base_port, !drive->master, starting_sector);
	outb(drive->io_base_port + SECTOR_COUNT_REGISTER_OFFSET, sector_count);

	_issue_ata_command(drive->io_base_port, READ_SECTORS_COMMAND);

	int read_bytes = 0;
	while (read_bytes < byte_count) {
		if (!_poll(drive->io_base_port)) {
			// a device  error has occured
			return -1;
		}
		for (int i = 0; i < BYTES_PER_SECTOR / 2; i++) {
			uint16_t data = inw(drive->io_base_port + DATA_REGISTER_OFFSET);
			//print_hex(data);
			if (read_bytes < byte_count) {
				buffer[read_bytes] = (char) (data & 0x00FF);
				buffer[read_bytes + 1] = (char) ((data & 0xFF00) >> 8);
			}
			read_bytes += 2;
		}
	}
	return read_bytes;
}

int _write_to_drive(char *buffer, ata_drive *drive, uint32_t starting_sector,
		uint32_t byte_count) {
	uint32_t sector_count =  (byte_count + BYTES_PER_SECTOR-1)  / BYTES_PER_SECTOR;
	if (starting_sector + sector_count >= drive->max_size) {
		return -1;
	}
	_select_lba28_address(drive->io_base_port, !drive->master, starting_sector);
	outb(drive->io_base_port + SECTOR_COUNT_REGISTER_OFFSET, sector_count);

	_issue_ata_command(drive->io_base_port, WRITE_SECTORS_COMMAND);

	int written_bytes = 0;
	while (written_bytes < byte_count) {
		if (!_poll(drive->io_base_port)) {
			// a device  error has occured
			return -1;
		}
		for (int i = 0; i < BYTES_PER_SECTOR / 2; i++) {
			uint16_t data = 0x0000;
			if (written_bytes < byte_count) {
				data |= buffer[written_bytes];
				data |= ((uint16_t) buffer[written_bytes + 1]) << 8;
			}
			outw(drive->io_base_port + DATA_REGISTER_OFFSET, data);
			written_bytes += 2;
		}
	}
	// from specification: This command may take longer than 30 s to complete.
	_cache_flush(drive);
	// the command takes time to set BSY, so I wait again
	_wait_for_device(drive->io_base_port);
	return written_bytes;
}

void _cache_flush(ata_drive *drive) {
	_issue_ata_command(drive->io_base_port, CACHE_FLUSH_COMMAND);
}

void _issue_ata_command(uint16_t io_base_port, uint16_t command) {

	_wait_for_device(io_base_port);
	outb(io_base_port + COMMAND_REGISTER_OFFSET, command);
}

void _wait_for_device(uint16_t io_base_port) {
	uint8_t status_register;
	//read five times because of 400ns delay, check osdev wiki ATA_PIO_Mode#400ns_delays
	for (int i = 0; i < 5; i++) {
		status_register = inb(io_base_port + STATUS_REGISTER_OFFSET);
	}

	while ((status_register & STATUS_REGISTER_BSY_FLAG)
			|| (status_register & STATUS_REGISTER_DRQ_FLAG)
			) {
		status_register = inb(io_base_port + STATUS_REGISTER_OFFSET);
		//TODO this is making debug wait forever
	}
}

int _poll(uint16_t io_base_port) {
	uint8_t status_register;
	//https://wiki.osdev.org/ATA_PIO_Mode#Polling_the_Status_vs._IRQs
	for (int i = 0; i < 5; i++) {
		status_register = inb(io_base_port + STATUS_REGISTER_OFFSET);
	}
	while (((status_register & STATUS_REGISTER_BSY_FLAG)
			|| !(status_register & STATUS_REGISTER_DRQ_FLAG))
			&& !((status_register & STATUS_REGISTER_ERR_FLAG)
					|| (status_register & STATUS_REGISTER_DF_FLAG))) {
		status_register = inb(io_base_port + STATUS_REGISTER_OFFSET);
	}
	if ((status_register & STATUS_REGISTER_ERR_FLAG)
			|| (status_register & STATUS_REGISTER_DF_FLAG)) {
		return 0;
	}
	return 1;
}

void _read_identify_output(ata_drive *drive) {
	for (int i = 0; i < 256; i++) {
		uint16_t data = inw(drive->io_base_port + DATA_REGISTER_OFFSET);
		if (i == 83) {
			//uint16_t 83: Bit 10 is set if the drive supports LBA48 mode.
			drive->lb48=data&1<<10;
		}
	}
}

int _read_size(ata_drive *drive) {
	//https://forum.osdev.org/viewtopic.php?f=1&t=14604
	return _read_size_lb28(drive);
}

int _read_size_lb28(ata_drive *drive) {
	_issue_ata_command(drive->io_base_port, READ_NATIVE_MAX_ADDRESS_COMMAND);
	int size = 0;
	uint8_t lba_low = inb(drive->io_base_port + LBA_LOW_REGISTER_OFFSET);
	uint8_t lba_mid = inb(drive->io_base_port + LBA_MID_REGISTER_OFFSET);
	uint8_t lba_high = inb(drive->io_base_port + LBA_HIGH_REGISTER_OFFSET);
	uint8_t drive_register = inb(drive->io_base_port + DRIVE_REGISTER_OFFSET);

	size = lba_low;
	size |= (((uint32_t) lba_mid) << 8);
	size |= (((uint32_t) lba_high) << 16);
	size |= (((uint32_t) (drive_register & 0x0F)) << 24);
	drive->max_size = size;

	return size;
}

void _select_lba28_address(uint16_t io_base_port, int slave,
		uint32_t starting_sector) {
	uint8_t select_drive_register = 0xA0;//two bits are always set:  https://wiki.osdev.org/ATA_PIO_Mode#Drive_.2F_Head_Register_.28I.2FO_base_.2B_6.29
	select_drive_register |= DRIVE_REGISTER_LBA_FLAG;
	if (slave) {
		select_drive_register |= DRIVE_REGISTER_SLAVE_FLAG;
	}
	select_drive_register |= (starting_sector >> 24) & 0x0F;
	uint8_t lba_low = starting_sector & 0xFF;
	uint8_t lba_mid = (starting_sector >> 8) & 0xFF;
	uint8_t lba_high = (starting_sector >> 16) & 0xFF;
	outb(io_base_port + DRIVE_REGISTER_OFFSET, select_drive_register);
	outb(io_base_port + LBA_LOW_REGISTER_OFFSET, lba_low);
	outb(io_base_port + LBA_MID_REGISTER_OFFSET, lba_mid);
	outb(io_base_port + LBA_HIGH_REGISTER_OFFSET, lba_high);
}

