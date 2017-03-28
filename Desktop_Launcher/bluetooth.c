//===================================================================
// Includes
//===================================================================
#include <stdint.h>
#include <unistd.h>
#include <string.h>

#include "bluetooth.h"
#include "serial.h"
#include "debug.h"
//===================================================================
// Defines
//===================================================================
#define BUFFER_SIZE 256

//===================================================================
// Constants
//===================================================================
unsigned char in_buffer[BUFFER_SIZE];

//===================================================================
// Private Function Definitions
//===================================================================
void get_cmd_response(size_t size)
{
	serial_read(BLUETOOTH, in_buffer, size);
	in_buffer[size] = '\0';
}

void flush_buffer(void)
{
	serial_read_timeout(BLUETOOTH, in_buffer, BUFFER_SIZE, 10);
	memset(in_buffer, 0, 256);
}

void enter_cmd_mode(void)
{
	serial_write(BLUETOOTH, "$$$", 3);
	get_cmd_response(5);
}

void exit_cmd_mode(void)
{
	serial_write(BLUETOOTH, "---\r\n", 5);
	get_cmd_response(5);
}


void set_name(void)
{
	serial_write(BLUETOOTH, "SN,DTL\r\n", 8);
	get_cmd_response(5);
}

void set_pin(void)
{
	serial_write(BLUETOOTH, "SA,4\r\n", 6);
	get_cmd_response(5);
	usleep(1000);
	serial_write(BLUETOOTH, "SP,9999\r\n", 9);
	get_cmd_response(5);
}

int check_connection(void)
{
	serial_write(BLUETOOTH, "GK\r\n", 4);
	get_cmd_response(7);

	if(in_buffer[0] == '1')
		return 1;
	else
		return 0;
}


//===================================================================
// Public Function Definitions
//===================================================================
void bluetooth_main(void)
{
	enter_cmd_mode();
	printf("cmd mode");
	while(1);
}

void init_bluetooth(void)
{
	flush_buffer();
	init_serial(BLUETOOTH);
	enter_cmd_mode();
	usleep(1000);
	set_name();
	usleep(1000);
	set_pin();
	usleep(1000);
	exit_cmd_mode();
}

int bluetooth_receive_message_timeout(uint8_t **message_buffer, unsigned long timeout)
{
	uint16_t read_bytes;
	read_bytes = serial_read_timeout(BLUETOOTH, in_buffer, 3, timeout);
	if(read_bytes != 3) {
		return 0;
	}

	uint16_t size = in_buffer[1]<<8 | in_buffer[2];
	if (size != 0)
		read_bytes += serial_read(BLUETOOTH, in_buffer + read_bytes, size);
	*message_buffer = in_buffer;
	return read_bytes;
}

void bluetooth_send_response(uint8_t response_message_id, uint8_t response_code)
{
	uint8_t out_buffer[5];
	out_buffer[0] = ID_RESPONSE;
	out_buffer[1] = 0;
	out_buffer[2] = 2;
	out_buffer[3] = response_message_id;
	out_buffer[4] = response_code;
	serial_write(BLUETOOTH, out_buffer, 5);
}

void bluetooth_send_image(uint8_t *image_data, uint16_t image_size)
{
	uint8_t out_buffer[3];
	out_buffer[0] = ID_MESG_IMAGE;
	out_buffer[1] = image_size>>8;
	out_buffer[2] = image_size & 0xFF;
	serial_write(BLUETOOTH, out_buffer, 3);
	serial_write(BLUETOOTH, image_data, image_size);
}

int bluetooth_connected(void)
{
	int result;

	enter_cmd_mode();
	usleep(1000);
	result = check_connection();
	exit_cmd_mode();

	return result;
}

