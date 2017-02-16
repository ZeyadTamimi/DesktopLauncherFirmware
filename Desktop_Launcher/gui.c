

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

//===================================================================
// Defines
//===================================================================
#define MAX_BUTTONS 8
#define TOGGLEABLE_BUTTON_NUMBER 3
#define MODE_BUTTON_NUMBER 3


//===================================================================
// Private Types
//===================================================================
typedef void (*function)();

typedef struct Button {
	int x0, y0;
	int x1, y1;
	button_type normal_button_type;
	int normal_button_color;
	button_type pressed_button_type;
	int pressed_button_color;
	function callback;
	int toggled;
} Button;


//===================================================================
// Global variables
//===================================================================
size_t button_array_size = MAX_BUTTONS;
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

void buttons_not_pressed(int start_index, int stop_index)
{
	int current_button_index;
	for (current_button_index = start_index; current_button_index < stop_index; current_button_index++)
	{
		Button *current_button = &button_array[current_button_index];
		draw_button(current_button->normal_button_type, current_button->normal_button_color);
		current_button->toggled = 0;
	}
}

//===================================================================
// Public Function Definitions
//===================================================================
int change_button_callback(int button_id, function callback)
{
	if (button_id >= MAX_BUTTONS)
		return 0;

	button_array[button_id].callback = callback;

	return 1;
}

int process_user_input(int timeout_usec)
{
    int pressed_button_index = poll_touch(timeout_usec);

    if (pressed_button_index == -1)
    {
    	buttons_not_pressed(TOGGLEABLE_BUTTON_NUMBER, MAX_BUTTONS);
        return 0;
    }

	Button *pressed_button = &button_array[pressed_button_index];

    if (pressed_button_index < TOGGLEABLE_BUTTON_NUMBER)
    	buttons_not_pressed(0, MODE_BUTTON_NUMBER);

    buttons_not_pressed(TOGGLEABLE_BUTTON_NUMBER, MAX_BUTTONS);

    if(pressed_button->toggled == 0)
	{
    	draw_button(pressed_button->pressed_button_type, pressed_button->pressed_button_color);
    	pressed_button->toggled = 1;
    	if (pressed_button->callback != NULL)
    		pressed_button->callback();
	}

	return 1;
}

void init_gui(){

    // Initialize all modules that are controlled by the GUI
     init_palette();
     init_touch();

    // Button initialization code
    Button manual_button;
    manual_button.x0 = 500;
    manual_button.x1 = 580;
    manual_button.y0 = 10;
    manual_button.y1 = 50;
    manual_button.normal_button_type = manual;
    manual_button.normal_button_color = CYAN;
    manual_button.pressed_button_type = manual_pressed;
    manual_button.pressed_button_color = WHITE;
    manual_button.toggled = 0;
    manual_button.callback = NULL;

    Button automatic_button;
    automatic_button.x0 = 590;
    automatic_button.x1 = 670;
    automatic_button.y0 = 10;
    automatic_button.y1 = 50;
    automatic_button.normal_button_type = automatic;
    automatic_button.normal_button_color = CYAN;
    automatic_button.pressed_button_type = automatic_pressed;
    automatic_button.pressed_button_color = WHITE;
    automatic_button.toggled = 0;
    automatic_button.callback = NULL;

    Button security_button;
    security_button.x0 = 680;
    security_button.x1 = 760;
    security_button.y0 = 10;
    security_button.y1 = 50;
    security_button.normal_button_type = security;
    security_button.normal_button_color = CYAN;
    security_button.pressed_button_type = security_pressed;
    security_button.pressed_button_color = WHITE;
    security_button.toggled = 0;
    security_button.callback = NULL;

	Button fire_button;
	fire_button.x0 = 580;
	fire_button.x1 = 620;
	fire_button.y0 = 430;
	fire_button.y1 = 470;
	fire_button.normal_button_type = fire;
	fire_button.normal_button_color = RED;
	fire_button.pressed_button_type = fire_pressed;
	fire_button.pressed_button_color = MAROON;
	fire_button.toggled = 0;
	fire_button.callback = NULL;

	Button left_button;
	left_button.x0 = 0;
	left_button.x1 = 50;
	left_button.y0 = 190;
	left_button.y1 = 290;
	left_button.normal_button_type = left;
	left_button.normal_button_color = LIME;
	left_button.pressed_button_type = left_pressed;
	left_button.pressed_button_color = FORESTGREEN;
	left_button.toggled = 0;
	left_button.callback = NULL;

	Button right_button;
	right_button.x0 = 750;
	right_button.x1 = 800;
	right_button.y0 = 190;
	right_button.y1 = 290;
	right_button.normal_button_type = right;
	right_button.normal_button_color = LIME;
	right_button.pressed_button_type = right_pressed;
	right_button.pressed_button_color = FORESTGREEN;
	right_button.toggled = 0;
	right_button.callback = NULL;

	Button up_button;
	up_button.x0 = 350;
	up_button.x1 = 450;
	up_button.y0 = 0;
	up_button.y1 = 50;
	up_button.normal_button_type = up;
	up_button.normal_button_color = LIME;
	up_button.pressed_button_type = up_pressed;
	up_button.pressed_button_color = FORESTGREEN;
	up_button.toggled = 0;
	up_button.callback = NULL;

	Button down_button;
	down_button.x0 = 350;
	down_button.x1 = 450;
	down_button.y0 = 430;
	down_button.y1 = 480;
	down_button.normal_button_type = down;
	down_button.normal_button_color = LIME;
	down_button.pressed_button_type = down_pressed;
	down_button.pressed_button_color = FORESTGREEN;
	down_button.toggled = 0;
	down_button.callback = NULL;

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


