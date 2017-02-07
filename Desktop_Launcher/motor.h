#ifndef MOTOR_H_
#define MOTOR_H_

#define MIN_MOTOR_SPEED 1
#define MAX_MOTOR_SPEED 3

void init_motors(void);
void move_up(void);
void move_down(void);
void move_left(void);
void move_right(void);
void set_motor_speed(void*);

#endif /* MOTOR_H_ */
