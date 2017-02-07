/*
 * serial.c
 *
 *  Created on: Jan 26, 2017
 *      Author: Zeyad
 */

//===================================================================
// Includes
//===================================================================
// STDLIB
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
// Project Includes
#include "serial.h"
#include "system.h"
#include "alt_types.h"
//===================================================================
// Defines
//===================================================================
// Register offsets
#define CONTROL_REGISTER_OFFSET 0
#define STATUS_REGISTER_OFFSET 0
#define TXDATA_REGISTER_OFFSET 2
#define RXDATA_REGISTER_OFFSET 2
#define BAUD_REGISTER_OFFSET 4
// Macro Functions
#define CONTROL(DEVICE)  (*(DEVICE + CONTROL_REGISTER_OFFSET))
#define STATUS(DEVICE)  (*(DEVICE + STATUS_REGISTER_OFFSET))
#define BAUD(DEVICE)  (*(DEVICE + BAUD_REGISTER_OFFSET))
#define TXDATA(DEVICE)  (*(DEVICE + TXDATA_REGISTER_OFFSET))
#define RXDATA(DEVICE)  (*(DEVICE + RXDATA_REGISTER_OFFSET))

#define WAIT_STEP_USEC 8

volatile unsigned char edge_capture;

//===================================================================
// ISR Serial Handler
//===================================================================
static void handle_serial_interrupts(void* context, alt_u32 id)
{
    volatile unsigned char* edge_capture_ptr = (volatile int*) context;
    (*edge_capture_ptr) = RXDATA(CAMERA);
}


//===================================================================
// Private Function Definitions
//===================================================================
void init_serial(volatile unsigned char* device)
{
    CONTROL(device) = 0x00;
    CONTROL(device) = 0x15;
    if (device == TOUCHSCREEN)
        BAUD(device) = 0x07;
    else if (device == CAMERA)
        BAUD(device) = 0x03;
    else
        BAUD(device) = 0x01;
}

void enable_device_recv_irq(volatile unsigned char* device)
{
    void* edge_capture_ptr = (void*) &edge_capture;
    // Enable IRQ
    //unsigned char clear_data = RXDATA(device);
    CONTROL(device) = 0x95;
    // Register the ISR
    alt_irq_register( TO_EXTERNAL_BUS_BRIDGE_0_IRQ, edge_capture_ptr,
            handle_serial_interrupts );
}

unsigned char test_irq(void)
{
    return edge_capture;
}

unsigned char serial_putchar(volatile unsigned char* device, char c)
{
    // poll Tx bit in 6850 status register. Wait for it to become '1'
    while (!(STATUS(device) & 0x02));
    // write 'c' to the 6850 TxData register to output the character
    TXDATA(device) = c;
    return c; // return c
}

unsigned char serial_getchar(volatile unsigned char* device)
{
    // poll Rx bit in 6850 status register. Wait for it to become '1'
    while (!test_for_received_data(device));
    // read received character from 6850 RxData register.
    return RXDATA(device);
}

//===================================================================
// Public Function Definitions
//===================================================================
size_t serial_write(volatile unsigned char* device, unsigned char * source_array, size_t source_array_size)
{
    size_t bytes_writen;
    for (bytes_writen = 0; bytes_writen < source_array_size; bytes_writen++)
        serial_putchar(device, source_array[bytes_writen]);

    return bytes_writen;
}

size_t serial_read(volatile unsigned char* device, unsigned char * source_array, size_t  source_array_size)
{
    size_t bytes_read;
    for (bytes_read = 0; bytes_read < source_array_size; bytes_read++)
    {
        source_array[bytes_read] = serial_getchar(device);
        //printf("Read the following byte: %04x\n", source_array[bytes_read]);
    }

    return bytes_read;
}

size_t serial_read_timeout( volatile unsigned char* device, unsigned char * source_array, size_t source_array_size, unsigned long timeout_usec)
{
    size_t bytes_read;
    for (bytes_read = 0; bytes_read < source_array_size; bytes_read++)
    {
        if (wait_for_received_data(device, timeout_usec))
            source_array[bytes_read] = serial_getchar(device);
        else
            break;
    }

    return bytes_read;
}

int test_for_received_data(volatile unsigned char* device)
{
  // Test Rx bit in 6850 serial comms chip status register
  // if RX bit is set, return TRUE, otherwise return FALSE
  return STATUS(device) & 0x01;
}

int wait_for_received_data(volatile unsigned char* device, unsigned long timeout_usec)
{
  // Test Rx bit in 6850 serial comms chip status register
  // if RX bit is set, return TRUE, otherwise return FALSE
  int data_available = 0;
  int elapsed_time = 0;
  data_available = test_for_received_data(device);
  while ((!data_available) && elapsed_time < timeout_usec)
  {
      usleep(WAIT_STEP_USEC);
      elapsed_time += WAIT_STEP_USEC;
      data_available = test_for_received_data(device);
  }
  return data_available;
}
