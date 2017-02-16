#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include<stdint.h>



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

void init_palette();
void print_image(unsigned char arr[320][240], int x_size, int y_size);
void draw_button(button_type button, int colour);
void program_palette(int PaletteNumber, uint32_t RGB);
void clear_screen(void);
void reticle(int colour);
#endif /* GRAPHICS_H_ */
