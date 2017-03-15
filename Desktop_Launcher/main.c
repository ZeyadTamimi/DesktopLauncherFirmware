// System Includes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
// Project Includes
#include "gui.h"
#include "touch_screen.h"
#include "motor.h"
#include "nano_jpeg.h"
#include "camera.h"
#include "graphics.h"
#include "wifi.h"
#include "time.h"
#include "bluetooth.h"

// Enable Image Processing
#define _NJ_INCLUDE_HEADER_ONLY

// Timeouts
#define SECURITY_POLL 500000
#define MANUAL_POLL 100000
#define AUTO_POLL 100000
#define USER_POLL 50000

// Motor Speeds
#define DEFAULT_MOTOR_SPEED 2

// Text Message
const char * motion_mesasge = "Someone is moving around!";

typedef enum desktop_launcher_mode
{
	MANUAL= 0x00,
	AUTO = 0x01,
	SECURITY = 0x02
}desktop_launcher_mode;

volatile desktop_launcher_mode current_mode;
volatile int bluetooth_value;  //CHANGED

//===================================================================
// Callbacks
//===================================================================
void mode_manual_callback()
{
	current_mode = MANUAL;
}


void mode_security_callback()
{
	current_mode = SECURITY;
}

void mode_auto_callback()
{
	current_mode = AUTO;
}

void photo_callback()
{
	uint8_t * jpeg_photo_buffer = NULL;
    write_processing_message(RED);
	take_picture();
	process_user_input(MANUAL_POLL);
	uint32_t photo_size = read_full_picture(&jpeg_photo_buffer);
	if (photo_size == 0)
	{
		printf("ERROR, CANT TAKE MANUAL PHOTO!");
		return;
	}
	process_user_input(MANUAL_POLL);
	int decode_return = njDecode(jpeg_photo_buffer, photo_size);
	if (decode_return == 0)
	{
		unsigned char *bitmap = njGetImage();
		process_user_input(MANUAL_POLL);
		unsigned char PMF[320][240];
		unsigned char value;
		int curr;
		for(curr = 0; curr < njGetImageSize()/3; curr++)
		{
			value = (bitmap[3*curr] & 0xC0) | (bitmap[3*curr+1] & 0xC0)>>2 |  (bitmap[3*curr+2] & 0xC0)>>4;
			PMF[curr%320][(int)(curr/320)] = (value)>>2;
		}
		process_user_input(MANUAL_POLL);
		print_image(PMF, 320, 240);
	}

	reticle(RED);
	resume_picture();
	free(jpeg_photo_buffer);
	erase_processing_message();
}

//===================================================================
// Modes
//===================================================================
void security_mode(void)
{
	/**********************
	 * CODE TO DISABLE BUTTONS/CALLBACKS
	 * SEND SMS WITH TIME
	 * SAVE PHOTO TO CLOUD
	 **********************/
	disable_button(UP_BUTTON);
	disable_button(DOWN_BUTTON);
	disable_button(LEFT_BUTTON);
	disable_button(RIGHT_BUTTON);
	disable_button(FIRE_BUTTON);
	disable_button(CAMERA_BUTTON);

	set_motion_detect(1);
	if (get_motion_detect())
		printf("ON\n");
	  else
		printf("OFF\n");

	set_motion_detect(1);
	int motion = motion_detected(SECURITY_POLL);
	if (motion)
	{
		motor_fire();
		send_sms("Someone is moving around!", 25);
	}

	process_user_input(SECURITY_POLL);

	set_motion_detect(0);
}

void auto_mode(void)
{
	disable_button(DOWN_BUTTON);
	disable_button(LEFT_BUTTON);
	disable_button(RIGHT_BUTTON);
	disable_button(FIRE_BUTTON);
	disable_button(CAMERA_BUTTON);
	disable_button(UP_BUTTON);

	uint8_t * jpeg_photo_buffer = NULL;

	// Move the motor
	move_left();
	usleep(1000000);
	stop_leftrght();

	// Check Motion
	set_motion_detect(1);
	if (get_motion_detect())
		printf("ON\n");
	  else
		printf("OFF\n");

	process_user_input(AUTO_POLL);
	int motion = motion_detected(2000000);
	if (motion)
	{
		motor_fire();
		send_sms("Someone is moving around!", 25);
	}

	process_user_input(AUTO_POLL);
	photo_callback();
	process_user_input(AUTO_POLL);
	set_motion_detect(0);
}

void manual_mode(void)
{
	enable_button(UP_BUTTON);
	enable_button(DOWN_BUTTON);
	enable_button(LEFT_BUTTON);
	enable_button(RIGHT_BUTTON);
	enable_button(FIRE_BUTTON);
	enable_button(CAMERA_BUTTON);
}

//ADDED--------------------------
void bluetooth_mode(void)
{
	disable_button(DOWN_BUTTON);
	disable_button(LEFT_BUTTON);
	disable_button(RIGHT_BUTTON);
	disable_button(FIRE_BUTTON);
	disable_button(CAMERA_BUTTON);
	disable_button(UP_BUTTON);
}

//-------------------------------

//===================================================================
// Bluetooth Command Handlers
//===================================================================
uint8_t move_command_time(uint8_t *bluetooth_rx_message, uint16_t size)
{
	// TODO Add size check
	uint32_t wait_time = 0;
	// Retrieve the wait time
	int byte_index;
	for (byte_index = 0; byte_index < SIZE_FIELD_COMMAND_MOVE_TIME; byte_index++)
		wait_time |= bluetooth_rx_message[SIZE_FIELD_HEADER + SIZE_FIELD_COMMAND_MOVE_DIR + byte_index] << (8 * (3 - byte_index));
	// Get the direction
	uint8_t direction = bluetooth_rx_message[SIZE_FIELD_HEADER];
	if (direction > MOVE_DOWN)
		return RESPONSE_INVALID_PARAM;
	// Move the motor
	move_direction(direction);
	usleep(wait_time);
	stop_leftrght();
	return RESPONSE_NO_ERROR;
}

uint8_t set_motor_speed_command(uint8_t *bluetooth_rx_message, uint16_t size)
{
	// TODO Add size check
	uint8_t new_speed = bluetooth_rx_message[SIZE_FIELD_HEADER];
	if (new_speed)
		return RESPONSE_INVALID_PARAM;
	set_motor_speed(new_speed);
	return RESPONSE_NO_ERROR;
}

void send_image_capture()
{
	uint8_t * jpeg_photo_buffer = NULL;
	take_picture();
	uint32_t photo_size = read_full_picture(&jpeg_photo_buffer);
	if (photo_size == 0)
	{
		printf("ERROR, CANT TAKE MANUAL PHOTO!");
		return;
	}
	resume_picture();
	printf("Image size is %ld\n", photo_size);
	bluetooth_send_image(jpeg_photo_buffer, photo_size);
	free(jpeg_photo_buffer);
}


void handle_command(uint8_t *bluetooth_rx_message, uint16_t size)
{
	uint8_t response_code;

	// Handle Command Messages
	switch (bluetooth_rx_message[0])
	{
		case ID_COMMAND_MOVE_TIME:
			response_code = move_command_time(bluetooth_rx_message, size);
			break;
		case ID_COMMAND_CHANGE_SPEED:
			response_code = set_motor_speed_command(bluetooth_rx_message, size);
			break;
		case ID_COMMAND_FIRE:
			motor_fire();
			break;
		default :
			response_code = RESPONSE_INVALID_COMMAND;
			break;
	}

	bluetooth_send_response(bluetooth_rx_message[0], response_code);
}

void handle_request(uint8_t *bluetooth_rx_message, uint16_t size)
{
	// Handle Command Messages
	switch (bluetooth_rx_message[3])
	{
		case ID_MESG_IMAGE:
			send_image_capture();
			break;
		default :
			bluetooth_send_response(bluetooth_rx_message[0], RESPONSE_INVALID_REQUEST);
			break;
	}
}


int main(void)
{

	// Mode Initialization
	current_mode = MANUAL;
	// Initialize the camera
	njInit();
	cam_init();
    camera_reset();
    printf(" Image Resolution was %d\n", get_image_resolution());
    set_image_resolution(FRAME_320x240);
	printf(" Image Resolution was %d\n", get_image_resolution());

	// Init the motors
	init_motors();
	set_motor_speed(DEFAULT_MOTOR_SPEED);

    // Gui init code
	init_gui();
	change_button_callback(LEFT_BUTTON, move_left);
	change_button_callback(RIGHT_BUTTON, move_right);
	change_button_callback(UP_BUTTON, move_up);
	change_button_callback(DOWN_BUTTON, move_down);
	change_button_callback(MANUAL_BUTTON, mode_manual_callback);
	change_button_callback(SECURITY_BUTTON, mode_security_callback);
	change_button_callback(AUTOMATIC_BUTTON, mode_auto_callback);

	//ADDED------------------------------------------------------------
	// TODO Add Bluetooth Callback
	//-----------------------------------------------------------------

	change_button_callback(FIRE_BUTTON, motor_fire);
	change_button_callback(CAMERA_BUTTON, photo_callback);

	// Init Bluetooth
	bluetooth_value = 1;
	init_bluetooth();
	uint8_t *bluetooth_rx_message;
	uint16_t message_size;



	// Main loop
	printf("IN MAIN!\n");
    while (1)
    {

		//process_user_input(USER_POLL);
		if (bluetooth_value) //CHANGED
		{
			message_size = bluetooth_receive_message_timeout(&bluetooth_rx_message, USER_POLL);
			if (message_size == 0)
				continue;

			if (bluetooth_rx_message[0] == ID_REQUEST)
				handle_request(bluetooth_rx_message, message_size);
			else
				handle_command(bluetooth_rx_message, message_size);

		}
		else
		{
			process_user_input(USER_POLL);
			switch (current_mode)
			{
			case SECURITY:
				security_mode();
				break;
			case MANUAL:
				manual_mode();
				break;
			case AUTO:
				auto_mode();
				break;
			}
		}
    }
}
