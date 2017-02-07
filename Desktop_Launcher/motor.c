//===================================================================
// Includes
//===================================================================
// System Includes
#include <assert.h>
// project Includes
#include "motor.h"

//===================================================================
// Defines
//===================================================================
#define PWM_CW1 (*((unsigned char*) 0x80000010))
#define PWM_CW2 (*((unsigned char*) 0x80000000))

#define kMinMotor 10 // min - right (1ms / 20ms)
#define kMaxMotor 33 // max - left  (2ms / 20ms)


//===================================================================
// Constants
//===================================================================

//===================================================================
// Global variables
//===================================================================
unsigned char motor_speed;

//===================================================================
// Private Function Definitions
//===================================================================

//===================================================================
// Public Function Definitions
//===================================================================
void init_motors(void)
{
	motor_speed = 0x01;
	PWM_CW1 = (kMinMotor + kMaxMotor ) / 2;
	PWM_CW2 = (kMinMotor + kMaxMotor ) / 2;
}

void move_up(void)
{
	if (PWM_CW1 < kMaxMotor)
    {
        PWM_CW1 += motor_speed;
        PWM_CW1 = PWM_CW1 < kMaxMotor ? PWM_CW1 : kMaxMotor;
        assert(PWM_CW1 <= kMaxMotor);
    }
}

void move_down(void)
{
    if (PWM_CW1 > kMinMotor)
    {
        PWM_CW1 -= motor_speed;
        PWM_CW1 = PWM_CW1 > kMinMotor ? PWM_CW1 : kMinMotor;
        assert(PWM_CW1 >= kMinMotor);
    }
}

void move_left(void)
{
    if (PWM_CW2 < kMaxMotor)
    {
        PWM_CW2 += motor_speed;
        PWM_CW2 = PWM_CW2 < kMaxMotor ? PWM_CW2 : kMaxMotor;
        assert(PWM_CW2 <= kMaxMotor);
    }
}

void move_right(void)
{
    if (PWM_CW2 > kMinMotor)
    {
        PWM_CW2 -= motor_speed;
        PWM_CW2 = PWM_CW2 > kMinMotor ? PWM_CW2 : kMinMotor;
        assert(PWM_CW1 >= kMinMotor);
    }
}

void set_motor_speed(void* speed)
{
	motor_speed = *((unsigned char*)speed);
	assert(MIN_MOTOR_SPEED <= motor_speed && motor_speed <= MAX_MOTOR_SPEED);
}
