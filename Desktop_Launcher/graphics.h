/*
 * graphics.h
 *
 *  Created on: Jan 31, 2017
 *      Author: deh
 */

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include<stdint.h>

//BUTTON CONSTANTS
#define LARROW_X1			50
#define LARROW_Y1			190
#define UARROW_X1			350
#define UARROW_Y1			50
#define RARROW_X1			750
#define RARROW_Y1			190
#define DARROW_X1			350
#define DARROW_Y1			430
#define ARROW_LENGTH		100

// defined constants representing colours pre-programmed into colour palette
// there are 256 colours but only 8 are shown below, we write these to the colour registers
//
// the header files "Colours.h" contains constants for all 256 colours
// while the course file ColourPaletteData.c contains the 24 bit RGB data
// that is pre-programmed into the palette
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

typedef enum button_type
{
	manual,
	automatic,
	security,
	fire,
	left,
	right,
	down,
	up,
	manual_pressed,
	automatic_pressed,
	security_pressed,
	fire_pressed,
	left_pressed,
	right_pressed,
	down_pressed,
	up_pressed
} button_type;

void print_image(unsigned char arr[320][240], int x_size, int y_size);

void draw_button(button_type button, int colour);
void write_a_pixel(int x, int y, int colour);
int  read_a_pixel(int x, int y);
void program_palette(int PaletteNumber, uint32_t RGB);
void horizontal_line(int x1, int y1, int length, int colour);
void vertical_line(int x1, int y1, int length, int colour);
void line(int x1, int y1, int x2, int y2, int colour);
void rectangle(int x1, int y1, int x2, int y2, int colour);
void right_triangle(int x1, int y1, int height, int colour);
void left_triangle(int x1, int y1, int height, int colour);
void down_triangle(int x1, int y1, int height, int colour);
void up_triangle(int x1, int y1, int height, int colour);
void circle(int x0, int y0, int radius, int colour);
void clear_screen(void);
void reticle(int colour);
void print_display(int arrow_colour, int menu_colour, int screen_colour,
				  int reticle_colour, int fire_colour);
void fill(int _x, int _y, int _FillColour, int _BoundaryColour);






#endif /* GRAPHICS_H_ */
