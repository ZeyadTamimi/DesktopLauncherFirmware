#include <stdio.h>      /* printf, scanf, NULL */
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "gui.h"
#include "touch_screen.h"
#include "motor.h"
#include "nano_jpeg.h"
#include "camera.h"
#include "graphics.h"

#define _NJ_INCLUDE_HEADER_ONLY

#define SECURITY_POLL 500000
#define MANUAL_POLL 100000
#define AUTO_POLL 100000
#define USER_POLL 50000

#define DEFAULT_MOTOR_SPEED 2
#define MAX_BYTES 128

#define SEND_DATA_SIZE 12
const char * send_data_command = "send_data";


typedef enum desktop_launcher_mode
{
	MANUAL= 0x00,
	AUTO = 0x01,
	SECURITY = 0x02
}desktop_launcher_mode;

volatile desktop_launcher_mode current_mode;
volatile desktop_launcher_mode past_mode;



int send_image_to_server(uint8_t *image_data, size_t image_size)
{
	// TODO FIX THIS PLS IM DED
	// Encode the length
	uint8_t length_image_data[SEND_DATA_SIZE + 4];
	snprintf(length_image_data, SEND_DATA_SIZE + 4, "%s(%04x)", send_data_command, image_size & 0xFFFF);
	printf("THE MESSAGE IS: %s",length_image_data);
	printf("WHY!");
	return 1;

	// Send the image data encoded in ascii  in MAX_BYTES packets
	uint8_t string_image_data[MAX_BYTES*2 + SEND_DATA_SIZE];
	while (image_size > MAX_BYTES)
	{

	}




	/*int current_character;
	for (current_character = 0; current_character < image_size; current_character++)
		string_image_data += sprintf(string_image_data, "%c", image_data[current_character]);
	string_image_data[image_size] = '\0';



	free(string_image_data);*/
}


void mode_manual_callback()
{
	past_mode = current_mode;
	current_mode = MANUAL;
}


void mode_security_callback()
{
	past_mode = current_mode;
	current_mode = SECURITY;
}

void mode_auto_callback()
{
	past_mode = current_mode;
	current_mode = AUTO;
}

/*void fire_callback()
{

}*/


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

	uint8_t * jpeg_photo_buffer = NULL;
	set_motion_detect(1);
	if (get_motion_detect())
		printf("ON\n");
	  else
		printf("OFF\n");

	set_motion_detect(1);
	int motion = motion_detected(SECURITY_POLL);
	if (motion)
	{
		// TODO SMS
	}

	int photo = take_picture();
	if (photo)
	{
		uint32_t photo_size = read_full_picture(&jpeg_photo_buffer);
		// TODO encode data
		// TODO send data;

	}

	process_user_input(SECURITY_POLL);

	free(jpeg_photo_buffer);
	resume_picture();
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
	usleep(200000);
	stop_leftrght();

	// Check Motion
	set_motion_detect(1);
	if (get_motion_detect())
		printf("ON\n");
	  else
		printf("OFF\n");

	process_user_input(AUTO_POLL);
	int motion = motion_detected(SECURITY_POLL);
	if (motion)
	{
		// TODO SMS
		// TODO FIRE
	}

	process_user_input(AUTO_POLL);
	int photo = take_picture();
	if (photo)
	{
		uint32_t photo_size = read_full_picture(&jpeg_photo_buffer);
		// TODO PRINT TO SCREEN
		// TODO encode data
		// TODO send data;
	}
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

int main (void)
{
	printf("HELLO WORLD!?!?!\n");
	send_image_to_server(NULL, 100);
	// Mode Initialization
	current_mode = MANUAL;
	past_mode = MANUAL;
	// Initialize the camera
	njInit();
	cam_init();
    camera_reset();
    printf(" Image Resolution was %d\n", get_image_resolution());
    set_image_resolution(FRAME_320x240);
	printf(" Image Resolution was %d\n", get_image_resolution());

	// Gui init code
	init_motors();
	set_motor_speed(DEFAULT_MOTOR_SPEED);
//	motor_test();

	// Gui init code
	init_gui();
	change_button_callback(LEFT_BUTTON, move_left);
	change_button_callback(RIGHT_BUTTON, move_right);
	change_button_callback(UP_BUTTON, move_up);
	change_button_callback(DOWN_BUTTON, move_down);
	change_button_callback(MANUAL_BUTTON, mode_manual_callback);
	change_button_callback(SECURITY_BUTTON, mode_security_callback);
	change_button_callback(AUTOMATIC_BUTTON, mode_auto_callback);
	change_button_callback(FIRE_BUTTON, motor_fire);
	change_button_callback(CAMERA_BUTTON, photo_callback);


	/**********************
	 * CALLBACK INIT CODE FOR FIRE HERE!
	 **********************/

    while (1)
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


