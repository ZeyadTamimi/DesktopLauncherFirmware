#ifndef GRAPHICS_H_
#define GRAPHICS_H_

//===================================================================
// Includes
//===================================================================
#include<stdint.h>


//===================================================================
// Defines
//===================================================================
// These numbers represent entries in the LCD color palette
#define	BLACK			0
#define	WHITE			63
#define	RED			    48
#define	LIME			12
#define	BLUE			3
#define	YELLOW			60
#define	CYAN			15
#define	MAGENTA			51
#define OLIVE			40
#define FORESTGREEN		8
#define MAROON			32
#define GREY			42

typedef enum button_type
{
	manual,
	automatic,
	security,
	bluetooth,
	fire,
	camera,
	left,
	right,
	down,
	up,
	manual_pressed,
	automatic_pressed,
	security_pressed,
	bluetooth_pressed,
	fire_pressed,
	camera_pressed,
	left_pressed,
	right_pressed,
	down_pressed,
	up_pressed
} button_type;

//===================================================================
// Public Function Declaration
//===================================================================
void init_palette();
void print_image(unsigned char arr[320][240], int x_size, int y_size);
void draw_button(button_type button, int colour);
void program_palette(int PaletteNumber, uint32_t RGB);
void clear_screen(void);
void reticle(int colour);
void write_processing_message(int message_colour);
void erase_processing_message(void);
void print_display( int arrow_colour, 
					int menu_colour, 
					int screen_colour,
					int reticle_colour, 
					int fire_colour, 
					int camera_colour);

#endif /* GRAPHICS_H_ */
