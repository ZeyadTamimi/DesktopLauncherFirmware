#ifndef GUI_H_
#define GUI_H_

//===================================================================
// Defines Function Declarations
//===================================================================
#define MANUAL_BUTTON 0
#define AUTOMATIC_BUTTON 1
#define SECURITY_BUTTON 2
#define FIRE_BUTTON 3
#define LEFT_BUTTON 4
#define RIGHT_BUTTON 5
#define UP_BUTTON 6
#define DOWN_BUTTON 7
#define CAMERA_BUTTON 8

typedef void (*function)();


//===================================================================
// Public Function Declarations
//===================================================================
void init_gui();
void process_user_input(int timeout_usec);
int change_button_callback(int button_id, function callback);
int disable_button(int button_id);
int enable_button(int button_id);
#endif /* GUI_H_ */
