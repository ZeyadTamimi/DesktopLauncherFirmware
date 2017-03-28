/*
 * serial.h
 *
 *  Created on: Jan 26, 2017
 *      Author: Zeyad
 */
#ifndef SERIAL_H_
#define SERIAL_H_

//===================================================================
// Includes
//===================================================================
#include <stdio.h>

//===================================================================
// Defines
//===================================================================
#define BLUETOOTH (volatile unsigned char *)(0x84000200)
#define WIFI (volatile unsigned char *)(0x84000220)
#define CAMERA (volatile unsigned char *)(0x84000210)
#define TOUCHSCREEN (volatile unsigned char *)(0x84000230)


//===================================================================
// Enums
//===================================================================
typedef enum baud
{
	BAUD_115200 = 0x01,
	BAUD_57600 = 0x02
} baud;
//===================================================================
// Public Function Declarations
//===================================================================
void init_serial(volatile unsigned char* device);
void set_device_baud(volatile unsigned char* device, baud baud_rate);
int test_for_received_data(volatile unsigned char* device);
int wait_for_received_data(volatile unsigned char* device, unsigned long timeout_usec);

unsigned char serial_getchar(volatile unsigned char* device);

size_t serial_write( volatile unsigned char* device, unsigned char * source_array, size_t source_array_size);
size_t serial_read( volatile unsigned char* device, unsigned char * source_array, size_t source_array_size);
size_t serial_read_timeout( volatile unsigned char* device, unsigned char * source_array, size_t source_array_size, unsigned long timeout_usec);

void enable_device_recv_irq(volatile unsigned char* device);
unsigned char test_irq(void);

#endif /* SERIAL_H_ */
