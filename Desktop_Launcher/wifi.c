//===================================================================
// Includes
//===================================================================
// System Includes
#include <stdint.h>
// Project Includes
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

const char * k_send_data_command = "send_data('%04x')\n";
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
