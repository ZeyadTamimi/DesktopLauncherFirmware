//===================================================================
// Includes
//===================================================================
#include <stdint.h>

#include "serial.h"
#include "wifi.h"
//===================================================================
// Defines
//===================================================================
#define MAX_BYTES 128
#define SEND_DATA_SIZE 14
#define SEND_SMS_SIZE 13
#define CONNECT_DB_SIZE 10
#define CLOSE_DB_SIZE 8

const char * k_send_data_command = "send_data('%s')\n";
const char * k_send_sms_command = "send_sms('%s')\n";
const char * k_connect_db_command = "connect()\n";
const char * k_close_db_connection = "close()\n";
//===================================================================
// Public Function Definitions
//===================================================================
void init_wifi(void)
{
	init_serial(WIFI);
}

void send_sms(const char* body, size_t len)
{
	char sms_message[SEND_SMS_SIZE + len + 1];
	int len_write = snprintf(sms_message, SEND_SMS_SIZE + len + 1, k_send_sms_command, body);
	printf("about to send %s, size %d", sms_message, SEND_SMS_SIZE + len);
	serial_write(WIFI, sms_message, len_write);
}

void connect_db(void)
{
	serial_write(WIFI, k_connect_db_command, CONNECT_DB_SIZE);
}

void close_db_connection(void)
{
	serial_write(WIFI, k_close_db_connection, CLOSE_DB_SIZE);
}

int send_data(uint8_t *image_data, size_t image_size)
{
	// Encode the length
	uint8_t length_image_data[SEND_DATA_SIZE + 5];
	int len_wrote = snprintf(length_image_data, SEND_DATA_SIZE + 5, "%s('%04x')",
							k_send_data_command, image_size & 0xFFFF);
	printf("THE MESSAGE IS: %s",length_image_data);
	serial_write(WIFI, length_image_data, len_wrote);

	int max_string_size = MAX_BYTES*2 + SEND_DATA_SIZE;
	uint8_t string_image_data[max_string_size + 1];

	// Send the image data encoded in ascii in MAX_BYTES packets
	while (image_size - MAX_BYTES*2 >= 0)
	{
		string_image_data[0] = 's';
		string_image_data[1] = 'e';
		string_image_data[2] = 'n';
		string_image_data[3] = 'd';
		string_image_data[4] = '_';
		string_image_data[5] = 'd';
		string_image_data[6] = 'a';
		string_image_data[7] = 't';
		string_image_data[8] = 'a';
		string_image_data[9] = '(';
		string_image_data[10] = '\'';
		int i = 11;
		for (i = 11; i < max_string_size - 3; i++)
		{
			snprintf(&string_image_data[i], 1, "%02x", image_data++[i]);
		}
		string_image_data[max_string_size - 3] = '\'';
		string_image_data[max_string_size - 2] = ')';
		string_image_data[max_string_size - 1] = '\n';
		image_size -= MAX_BYTES*2;
		serial_write(WIFI, string_image_data, max_string_size);
	}

	// last run through
	if (image_size > 0)
	{
		string_image_data[0] = 's';
		string_image_data[1] = 'e';
		string_image_data[2] = 'n';
		string_image_data[3] = 'd';
		string_image_data[4] = '_';
		string_image_data[5] = 'd';
		string_image_data[6] = 'a';
		string_image_data[7] = 't';
		string_image_data[8] = 'a';
		string_image_data[9] = '(';
		string_image_data[10] = '\'';
		int i = 11;
		for (i = 11; i < image_size + 11; i++)
		{
			snprintf(&string_image_data[i], 1, "%02x", image_data++[i]);
		}
		string_image_data[image_size + 11] = '\'';
		string_image_data[image_size + 12] = ')';
		string_image_data[image_size + 13] = '\n';
		serial_write(WIFI, string_image_data, image_size + 13);
	}

	return 1;

	/*int current_character;
	for (current_character = 0; current_character < image_size; current_character++)
		string_image_data += sprintf(string_image_data, "%c", image_data[current_character]);
	string_image_data[image_size] = '\0';



	free(string_image_data);*/
}
