//===================================================================
// Includes
//===================================================================
// System Includes
#include <stdint.h>
// Project Includes
#include "serial.h"
#include "touch_screen.h"
#include "debug.h"

//===================================================================
// Defines
//===================================================================
//  Response Codes
#define STATUS_OK 0x00
// Commands IDs
#define PEN_UP 0x80
#define PEN_DOWN 0x81
// Conversion Factors
#define X_CONVERSION_FACTOR 5.12
#define Y_CONVERSION_FACTOR 8.53

//===================================================================
// Constants
//===================================================================
unsigned char ENABLE_TOUCH_COMMAND[] = {0x55, 0x01, 0x12};

//===================================================================
// Utility Functions
//===================================================================
int get_raw_coordinates(unsigned char upper_byte, unsigned char lower_byte)
{
    return ((upper_byte & 0x1F) << 0x07) | (lower_byte & 0x7F);
}

/*!
 * \brief Verifies that the 4 byte touch report body is valid.
 * \warning Only guarantees that the 4 bytes are not header bytes!
 * \param [in] touch_report, The command byte sent.
 * \return This function returns 1 if the response from the device is well
 *         formed, else it returns 0.
 */
int verify_touch_report(unsigned char * touch_report, int size)
{
    int touch_report_index;
    for (touch_report_index = 0; touch_report_index < size; touch_report_index++)
    {
        if (touch_report[touch_report_index] & 0x80)
            return 0;
    }

    return 1;
}

//===================================================================
// Public Function Definitions
//===================================================================
int init_touch(void)
{
    // Program 6850 and baud rate generator to communicate with touchscreen
    init_serial(TOUCHSCREEN);
    // send touchscreen controller an "enable touch" command
    serial_write(TOUCHSCREEN, ENABLE_TOUCH_COMMAND, 0x03);
    // Wait for response
    unsigned char touch_enable_response[0x04];
    serial_read(TOUCHSCREEN, touch_enable_response, 0x04);
    if ((touch_enable_response[2] != STATUS_OK) || (touch_enable_response[3] != 0x12))
            return 0;

    return 1;
}

int ScreenTouched( void )
{
    return test_for_received_data(CAMERA);
}

void WaitForTouch()
{
    while(!ScreenTouched());
}

Point get_press_timeout(unsigned long timeout_usec)
{
    Point press_point = {.x = INVALID_POINT_DATA, .y = INVALID_POINT_DATA};
    int num_bytes = 0;
    // wait for a pen down command then return the X,Y coord of the point
    // Wait for the first byte to indicate the type of message.
    unsigned char received_event_byte = 0x00;
    while (received_event_byte != PEN_DOWN)
    {
        num_bytes = serial_read_timeout(TOUCHSCREEN, &received_event_byte,
                                        0x01, timeout_usec);
        if (num_bytes != 1)
            return press_point;
    }

    // Receive the rest of the message Pen Down Message
    unsigned char touch_report[0x04];
    num_bytes = serial_read_timeout(TOUCHSCREEN, &touch_report, 0x04, timeout_usec);

    if (num_bytes != 0x04)
        return press_point;

    if (verify_touch_report(touch_report, num_bytes))
    {
        press_point.x =  (get_raw_coordinates(touch_report[1], touch_report[0]))/ X_CONVERSION_FACTOR;
        press_point.y =  (get_raw_coordinates(touch_report[3], touch_report[2]))/ Y_CONVERSION_FACTOR;
    }
    // calibrated correctly so that it maps to a pixel on screen
    return press_point;
}

Point get_press(void)
{
    Point press_point = {.x = -1, .y = -1};
    // wait for a pen down command then return the X,Y coord of the point
    // Wait for the first byte to indicate the type of message.
    unsigned char received_event_byte = 0x00;
    while (received_event_byte != PEN_DOWN)
        serial_read(TOUCHSCREEN, &received_event_byte, 0x01);
    // Receive the rest of the message Pen Down Message
    unsigned char touch_report[0x04];
    serial_read(TOUCHSCREEN, &touch_report, 0x04);
    if (verify_touch_report(touch_report, 0x04))
    {
        press_point.x =  (get_raw_coordinates(touch_report[1], touch_report[0]))/ X_CONVERSION_FACTOR;
        press_point.y =  (get_raw_coordinates(touch_report[3], touch_report[2]))/ Y_CONVERSION_FACTOR;
    }
    // calibrated correctly so that it maps to a pixel on screen
    return press_point;
}

Point get_release(void)
{
    Point release_point = {.x = -1, .y = -1};
    // wait for a pen down command then return the X,Y coord of the point
    // Wait for the first byte to indicate the type of message.
    unsigned char received_event_byte = 0x00;
    while (received_event_byte != PEN_UP)
        serial_read(TOUCHSCREEN, &received_event_byte, 0x01);
    // Receive the rest of the message Pen Down Message
    unsigned char touch_report[0x04];
    serial_read(TOUCHSCREEN, &touch_report, 0x04);
    if (verify_touch_report(touch_report, 0x04))
    {
        release_point.x =  (get_raw_coordinates(touch_report[1], touch_report[0]))/ X_CONVERSION_FACTOR;
        release_point.y =  (get_raw_coordinates(touch_report[3], touch_report[2]))/ Y_CONVERSION_FACTOR;
    }
    // calibrated correctly so that it maps to a pixel on screen
    return release_point;
}
