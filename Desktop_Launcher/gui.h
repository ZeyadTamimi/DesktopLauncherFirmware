#ifndef GUI_H_
#define GUI_H_

//===================================================================
// Defines Function Declarations
//===================================================================

#define MANUAL_BUTTON 0
#define AUTOMATIC_BUTTON 1
#define SECURITY_BUTTON 2
//ADDED----------------------
#define BLUETOOTH_BUTTON 3
//---------------------------
#define FIRE_BUTTON 4   //CHANGED
#define LEFT_BUTTON 5   //CHANGED
#define RIGHT_BUTTON 6  //CHANGED
#define UP_BUTTON 7     //CHANGED
#define DOWN_BUTTON 8   //CHANGED
#define CAMERA_BUTTON 9 //CHANGED

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
