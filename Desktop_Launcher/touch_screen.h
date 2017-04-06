#ifndef TOUCH_SCREEN_H_
#define TOUCH_SCREEN_H_

//===================================================================
// Defines
//===================================================================
#define INVALID_POINT_DATA -1

//===================================================================
// Public Data types
//===================================================================
/* a data type to hold a point/coord */
struct Point;
typedef struct Point { int x, y; } Point ;

//===================================================================
// Public Function Declaration
//===================================================================
int init_touch(void);
Point get_press(void);
Point get_release(void);
Point get_press_timeout(unsigned long timeout_usec);

#endif /* TOUCH_SCREEN_H_ */
