#include <stdint.h>

#ifndef MOTOR_H_
#define MOTOR_H_

#define MOVE_RIGHT 0
#define MOVE_LEFT 1
#define MOVE_UP 2
#define MOVE_DOWN 3

#define MIN_MOTOR_SPEED 1
#define MAX_MOTOR_SPEED 9

int motor_test();
void init_motors(void);
void move_direction(int direction);
void move_up(void);
void move_down(void);
void move_left(void);
void move_right(void);
void stop_leftrght(void);
void move_updown_angle(int16_t angle);
void move_leftright_angle(int16_t angle);
void motor_load(void);
void motor_fire(void);
// use around 1 - 9
void set_motor_speed(int speed_multiplier);

// Use this to debug
// motor - 0 for updown
//         1 for l/r 360,
//         2 for fire
void set_direct_PWM(int motor, int pwm_counter);

#endif /* MOTOR_H_ */
