#include <assert.h>

#include "motor.h"
#include <unistd.h>
#include <stdint.h>

//===================================================================
// Memory Mapped Defines
//===================================================================
#define PWM_CW180 (*((unsigned short*) 0x80000010)) // u/d GPIO 40
#define PWM_CW360 (*((unsigned short*) 0x80000000)) // l/r GPIO 38
#define PWM_CWFIRE (*((unsigned short*) 0x80000060)) // fire GPIO 36

//===================================================================
// Motor Limits
//===================================================================
#define kCW180_MAX 4900
#define kCW180_MIN 2900
#define kCW180_MID 3900
//2000-8000

#define kCWFIRE_MAX 5750
#define kCWFIRE_MIN 2000
#define kCWFIRE_MID 3000

#define kRANGE360 32

//===================================================================
// Constants
//===================================================================
#define UP_DOWN_ANGLE_PWM 33
#define LEFT_RIGHT_ANGLE_TIME 18000

//===================================================================
// Global variables
//===================================================================
unsigned short updown_speed;
unsigned short leftright_speed;
int bluetooth_enabled = 0;
int bluetooth_multiplier = 15;
int up_multiplier = 1;
int down_multiplier = 1;

uint16_t kCW360_MID = 4113;

//===================================================================
// Private Function Definitions
//===================================================================
int motor_test()
{
	init_motors();
	while (1)
	{
		/*
		unsigned char c;
		printf("enter: ");
		scanf("%c", &c);
		if (c == 'w') move_up();
		else if (c == 'a') move_left();
		*/

		int pwm_counter;
		printf("enter: ");
		scanf("%d", &pwm_counter);
		set_direct_PWM(2, pwm_counter);

	}
	return 0;
}
//===================================================================
// Public Function Definitions
//===================================================================
void init_motors(void)
{
    set_motor_speed(2);
    PWM_CW180 = kCW180_MID;
    PWM_CW360 = kCW360_MID;
    PWM_CWFIRE = kCWFIRE_MID;

    //for setting up 360 center
    if(0)
    {
    	printf("360 motor init\n");
    	uint16_t d = 1;
    	while(d != 0)
    	{
			move_leftright_angle(45);
			usleep(500000);
			move_leftright_angle(-45);
			usleep(500000);
			printf("enter delta: ");
			scanf("%d", &d);
			kCW360_MID -= d;
			printf("new mid: %d\n", kCW360_MID);
    	}

        uint16_t a = 1;
        while(a != 0)
        {
        	printf("angle: ");
        	scanf("%d", &a);
        	move_updown_angle(a);
        }
    }
}

void move_direction(int direction)
{
	bluetooth_enabled = 1;
	switch(direction)
	{
	case MOVE_UP:
		move_up();
		break;
	case MOVE_DOWN:
		move_down();
		break;
	case MOVE_LEFT:
		move_left();
		break;
	case MOVE_RIGHT:
		move_right();
		break;
	}

	bluetooth_enabled = 0;
}


void move_up(void)
{
    if (PWM_CW180 > kCW180_MIN)
    {
        uint16_t PWM_NEW = PWM_CW180;
        if(bluetooth_enabled)
        {
            PWM_NEW -= updown_speed * bluetooth_multiplier * up_multiplier;
            up_multiplier = 1;
            down_multiplier = 3;
        }
        else
            PWM_NEW -= updown_speed;
        PWM_CW180 = PWM_NEW > kCW180_MIN ? PWM_NEW : kCW180_MIN;
        assert(PWM_CW180 >= kCW180_MIN);
    }
}

void move_down(void)
{
    if (PWM_CW180 < kCW180_MAX)
    {
        uint16_t PWM_NEW = PWM_CW180;
        if(bluetooth_enabled)
        {
            PWM_NEW += updown_speed * bluetooth_multiplier * down_multiplier;
        	up_multiplier = 3;
        	down_multiplier = 1;
        }
        else
            PWM_NEW += updown_speed;
        PWM_CW180 = PWM_NEW < kCW180_MAX ? PWM_NEW : kCW180_MAX;
        assert(PWM_CW180 <= kCW180_MAX);
    }
}

void move_left(void)
{
	PWM_CW360 = kCW360_MID + kRANGE360 * leftright_speed;
}

void move_right(void)
{
	PWM_CW360 = kCW360_MID - kRANGE360 * leftright_speed;
}

void move_updown_angle(int16_t angle)
{
	if (angle < -180)
		angle = -180;
	else if (angle > 180)
		angle = 180;

	int32_t PWM_NEW = PWM_CW180;
	PWM_NEW = PWM_NEW - angle * UP_DOWN_ANGLE_PWM;
	if(PWM_NEW > kCW180_MAX)
	{
		PWM_CW180 = kCW180_MAX;
	}
	else if(PWM_NEW < kCW180_MIN)
	{
		PWM_CW180 = kCW180_MIN;
	}
	else
	{
		PWM_CW180 = PWM_NEW;
	}
}

void move_leftright_angle(int16_t angle)
{
	if (angle < -90)
		angle = -90;
	else if (angle > 90)
		angle = 90;

	if (angle > 0)
	{
		PWM_CW360 = kCW360_MID - kRANGE360 * 3;
	}
	else
	{
		PWM_CW360 = kCW360_MID + kRANGE360 * 3;
		angle *= -1;
	}

	usleep(angle * LEFT_RIGHT_ANGLE_TIME);
	stop_leftrght();
}

void stop_leftrght(void)
{
    PWM_CW360 = kCW360_MID;
}

void motor_load(void)
{
    PWM_CWFIRE = kCWFIRE_MID;
}

void motor_fire(void)
{
    PWM_CWFIRE = kCWFIRE_MAX;
    usleep(500000);
    PWM_CWFIRE = kCWFIRE_MID;
}

void set_motor_speed(int speed_multiplier)
{
    updown_speed = 5 * speed_multiplier;
    leftright_speed = speed_multiplier;
}

int get_motor_speed()
{
	return leftright_speed;
}


// Use this to debug!
void set_direct_PWM(int motor, int pwm_counter)
{
    if (motor == 0) PWM_CW180 = pwm_counter;
    else if (motor == 1) PWM_CW360 = pwm_counter;
    else if (motor == 2) PWM_CWFIRE = pwm_counter;
}
