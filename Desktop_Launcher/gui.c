

//===================================================================
// Includes
//===================================================================
// System Includes
#include <stdio.h>
#include <stdlib.h>
// Project Includes
#include "gui.h"
#include "graphics.h"
#include "touch_screen.h"
#include "motor.h"
#include "camera.h"

//===================================================================
// Defines
//===================================================================
#define MAX_BUTTONS 8
#define MANUAL_BUTTON 0
#define AUTOMATIC_BUTTON 1
#define SECURITY_BUTTON 2
#define FIRE_BUTTON 3
#define LEFT_BUTTON 4
#define RIGHT_BUTTON 5
#define UP_BUTTON 6
#define DOWN_BUTTON 7


//===================================================================
// Private Types
//===================================================================
typedef void (*function)();

typedef struct Button {

	int x0;
	int x1;
	int y0;
	int y1;
	button_type this_button_type;
	button_type pressed_button_type;
	function func;
	int toggled;
} Button;

void manual_callback(void);
void automatic_callback(void);
void security_callback(void);
void fire_callback(void);
void left_callback(void);
void right_callback(void);
void up_callback(void);
void down_callback(void);
void buttons_not_pressed(void);

//===================================================================
// Global variables
//===================================================================
size_t button_array_size = 8;
Button button_array[MAX_BUTTONS];

//===================================================================
// Private Function Definitions
//===================================================================

int check_if_pressed(Button button_under_test, Point press_point)
{
    if ((press_point.x >= button_under_test.x0) &&
        (press_point.x <= button_under_test.x1) &&
        (press_point.y >= button_under_test.y0) &&
        (press_point.y <= button_under_test.y1))
            return 1;

    return 0;
}

int poll_touch(int timeout_usec)
{
    Point touch_point = get_press_timeout(timeout_usec);
    if (touch_point.x == INVALID_POINT_DATA){

        return -1;
    }
    //touch_point = get_release();


    int button_index;
    for (button_index = 0; button_index < button_array_size; button_index++)
    {
        if(check_if_pressed(button_array[button_index], touch_point))
            return button_index;
    }
    return -1;
}


void retrieve_and_process_image(void)
{
    take_picture();
    uint32_t size = frame_length();
    uint32_t len = size;
    printf("Frame size: %ld\n", size);

    int current_index = 0;
    int i;
    uint8_t *jpeg_buffer = malloc((sizeof(uint8_t)*size) + 10);
    uint8_t *buffer;

    /*while (size > 64) {
        // read 32 bytes at a time;
        buffer = read_picture(64);
        size -= 64;
        for(i=0;i<64;i++)
            jpeg_buffer[current_index+i] = *(buffer+i);
        current_index += 64;
    }

    buffer = read_picture(size);
    for(i=0;i<size;i++)
        jpeg_buffer[current_index+i] = *(buffer+i);*/

    while (size > 64) {
            // read 32 bytes at a time;
            read_picture_to_ptr(jpeg_buffer, 64);
            size -= 64;
        }

    read_picture_to_ptr(jpeg_buffer, size);


    resume_picture();
    printf("DECODING\n");

    //print_byte_array(jpeg_buffer, len);
    printf("The value of decode is: %d\n", njDecode(jpeg_buffer,len));


    unsigned char *bitmap = njGetImage();
    unsigned char PMF[320][240];
    unsigned char value;
    int curr;
    for(curr = 0; curr < njGetImageSize()/3; curr++)
    {
        value = (bitmap[3*curr] & 0xC0) | (bitmap[3*curr+1] & 0xC0)>>2 |  (bitmap[3*curr+2] & 0xC0)>>4;
        PMF[curr%320][(int)(curr/320)] = (value)>>2;
    }

    print_image(PMF, 320, 240);
    free(jpeg_buffer);
}

//===================================================================
// Public Function Definitions
//===================================================================
 void init_gui(){

    // Initialize all modules that are controlled by the GUI
     init_palette();
     init_touch();
     init_motors();
     int speed = 3;
     set_motor_speed(&speed);

    // Button initialization code
    Button manual_button;
    manual_button.x0 = 500;
    manual_button.x1 = 580;
    manual_button.y0 = 10;
    manual_button.y1 = 50;
    manual_button.this_button_type = manual;
    manual_button.pressed_button_type = manual_pressed;
    manual_button.toggled = 0;
    manual_button.func = manual_callback;

    Button automatic_button;
    automatic_button.x0 = 590;
    automatic_button.x1 = 670;
    automatic_button.y0 = 10;
    automatic_button.y1 = 50;
    automatic_button.this_button_type = automatic;
    automatic_button.pressed_button_type = automatic_pressed;
    automatic_button.toggled = 0;
    automatic_button.func = automatic_callback;

    Button security_button;
    security_button.x0 = 680;
    security_button.x1 = 760;
    security_button.y0 = 10;
    security_button.y1 = 50;
    security_button.this_button_type = security;
    security_button.pressed_button_type = security_pressed;
    security_button.toggled = 0;
    security_button.func = security_callback;

	Button fire_button;
	fire_button.x0 = 580;
	fire_button.x1 = 620;
	fire_button.y0 = 430;
	fire_button.y1 = 470;
	fire_button.this_button_type = fire;
	fire_button.pressed_button_type = fire_pressed;
	fire_button.toggled = 0;
	fire_button.func = fire_callback;

	Button left_button;
	left_button.x0 = 0;
	left_button.x1 = 50;
	left_button.y0 = 190;
	left_button.y1 = 290;
	left_button.this_button_type = left;
	left_button.pressed_button_type = left_pressed;
	left_button.toggled = 0;
	left_button.func = left_callback;

	Button right_button;
	right_button.x0 = 750;
	right_button.x1 = 800;
	right_button.y0 = 190;
	right_button.y1 = 290;
	right_button.this_button_type = right;
	right_button.pressed_button_type = right_pressed;
	right_button.toggled = 0;
	right_button.func = right_callback;

	Button up_button;
	up_button.x0 = 350;
	up_button.x1 = 450;
	up_button.y0 = 0;
	up_button.y1 = 50;
	up_button.this_button_type = up;
	up_button.pressed_button_type = up_pressed;
	up_button.toggled = 0;
	up_button.func = up_callback;

	Button down_button;
	down_button.x0 = 350;
	down_button.x1 = 450;
	down_button.y0 = 430;
	down_button.y1 = 480;
	down_button.this_button_type = down;
	down_button.pressed_button_type = down_pressed;
	down_button.toggled = 0;
	down_button.func = down_callback;

	button_array[MANUAL_BUTTON] = manual_button;
	button_array[AUTOMATIC_BUTTON] = automatic_button;
	button_array[SECURITY_BUTTON] = security_button;
	button_array[FIRE_BUTTON] = fire_button;
	button_array[LEFT_BUTTON] = left_button;
	button_array[RIGHT_BUTTON] = right_button;
	button_array[UP_BUTTON] = up_button;
	button_array[DOWN_BUTTON] = down_button;

    print_display(LIME,CYAN,WHITE,RED,RED);

}

int proccess_buttons(int timeout_usec){
    int pressed_button = poll_touch(timeout_usec);

    if (pressed_button < 4)
    {
    	buttons_not_pressed();
    }
    if (pressed_button == -1)
    {
        return 0;
    }
    else
    {
        button_array[pressed_button].func();
        return 1;
    }
}

void manual_callback()
{
	if(button_array[MANUAL_BUTTON].toggled == 0)
	{
		draw_button(manual_pressed,WHITE);
		draw_button(automatic,CYAN);
		draw_button(security,CYAN);
		button_array[MANUAL_BUTTON].toggled = 1;
		button_array[AUTOMATIC_BUTTON].toggled = 0;
		button_array[SECURITY_BUTTON].toggled = 0;
		printf("GOING TO MANUAL MODE\n");
		//go_to_security_mode();
	}

}

void automatic_callback()
{
	if(button_array[AUTOMATIC_BUTTON].toggled == 0)
	{
		draw_button(manual,CYAN);
		draw_button(automatic_pressed,WHITE);
		draw_button(security,CYAN);
		button_array[MANUAL_BUTTON].toggled = 0;
		button_array[AUTOMATIC_BUTTON].toggled = 1;
		button_array[SECURITY_BUTTON].toggled = 0;
		printf("GOING TO AUTOMATIC MODE\n");
		//go_to_automatic_mode();
	}
}

void security_callback()
{
	if(button_array[SECURITY_BUTTON].toggled == 0)
	{
		draw_button(manual,CYAN);
		draw_button(automatic,CYAN);
		draw_button(security_pressed,WHITE);
		button_array[MANUAL_BUTTON].toggled = 0;
		button_array[AUTOMATIC_BUTTON].toggled = 0;
		button_array[SECURITY_BUTTON].toggled = 1;
		//go_to_security_mode();
		printf("GOING TO SECURITY MODE\n");
	}
}

void fire_callback()
{
	if (button_array[FIRE_BUTTON].toggled == 0)
	{
		draw_button(fire_pressed,MAROON);
		printf("FIRE\n");
		button_array[FIRE_BUTTON].toggled = 1;
	}
	retrieve_and_process_image();
}

void left_callback()
{
    if (button_array[LEFT_BUTTON].toggled == 0)
	{
		draw_button(left_pressed,FORESTGREEN);
		draw_button(right,LIME);
		draw_button(up,LIME);
		draw_button(down,LIME);
		move_left();
		printf("ROTATING LEFT\n");

		button_array[LEFT_BUTTON].toggled = 1;
		button_array[RIGHT_BUTTON].toggled = 0;
		button_array[UP_BUTTON].toggled = 0;
		button_array[DOWN_BUTTON].toggled = 0;
	}
}
void right_callback()
{
    if(button_array[RIGHT_BUTTON].toggled == 0)
	{
		draw_button(left,LIME);
		draw_button(right_pressed,FORESTGREEN);
		draw_button(up,LIME);
		draw_button(down,LIME);
		move_right();
		printf("ROTATING RIGHT\n");

		button_array[LEFT_BUTTON].toggled = 0;
		button_array[RIGHT_BUTTON].toggled = 1;
		button_array[UP_BUTTON].toggled = 0;
		button_array[DOWN_BUTTON].toggled = 0;
	}
}
void up_callback()
{
	if(button_array[UP_BUTTON].toggled == 0)
	{
		draw_button(left,LIME);
		draw_button(right,LIME);
		draw_button(up_pressed,FORESTGREEN);
		draw_button(down,LIME);
		move_up();
		printf("ROTATING UP\n");

		button_array[LEFT_BUTTON].toggled = 0;
		button_array[RIGHT_BUTTON].toggled = 0;
		button_array[UP_BUTTON].toggled = 1;
		button_array[DOWN_BUTTON].toggled = 0;
	}
}
void down_callback()
{
	if(button_array[DOWN_BUTTON].toggled == 0)
	{
		draw_button(left,LIME);
		draw_button(right,LIME);
		draw_button(up,LIME);
		draw_button(down_pressed,FORESTGREEN);
		move_down();
		printf("ROTATING DOWN\n");

		button_array[LEFT_BUTTON].toggled = 0;
		button_array[RIGHT_BUTTON].toggled = 0;
		button_array[UP_BUTTON].toggled = 0;
		button_array[DOWN_BUTTON].toggled = 1;
	}
}


void buttons_not_pressed()
{
	draw_button(fire,RED);
	draw_button(left,LIME);
	draw_button(right,LIME);
	draw_button(up,LIME);
	draw_button(down,LIME);

	button_array[FIRE_BUTTON].toggled = 0;
	button_array[LEFT_BUTTON].toggled = 0;
	button_array[RIGHT_BUTTON].toggled = 0;
	button_array[UP_BUTTON].toggled = 0;
	button_array[DOWN_BUTTON].toggled = 0;
}


