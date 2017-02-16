#include <stdio.h>
#include <stdint.h>
#include "fonts.h"
#include "graphics.h"

// graphics registers all address begin with '8' so as to by pass data cache on NIOS
#define GraphicsCommandReg   		(*(volatile unsigned short int *)(0x84000000))
#define GraphicsStatusReg   		(*(volatile unsigned short int *)(0x84000000))
#define GraphicsX1Reg   			(*(volatile unsigned short int *)(0x84000002))
#define GraphicsY1Reg   			(*(volatile unsigned short int *)(0x84000004))
#define GraphicsX2Reg   			(*(volatile unsigned short int *)(0x84000006))
#define GraphicsY2Reg   			(*(volatile unsigned short int *)(0x84000008))
#define GraphicsColourReg   		(*(volatile unsigned short int *)(0x8400000E))
#define GraphicsBackGroundColourReg   	(*(volatile unsigned short int *)(0x84000010))

/************************************************************************************************
** This macro pauses until the graphics chip status register indicates that it is idle
***********************************************************************************************/
#define WAIT_FOR_GRAPHICS		while((GraphicsStatusReg & 0x0001) != 0x0001);

// #defined constants representing values we write to the graphics 'command' register to get
// it to draw something

#define DrawHLine		1
#define DrawVLine		2
#define DrawLine		3
#define	PutAPixel		0xA
#define	GetAPixel		0xB
#define	ProgramPaletteColour    0x10




#define XRES				800
#define YRES				480
#define FONT2_XPIXELS		10				// width of Font2 characters in pixels (no spacing)
#define FONT2_YPIXELS		14				// height of Font2 characters in pixels (no spacing)



#define PICTURE_X_OFFSET 240
#define PICTURE_Y_OFFSET 120

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
/*
 * This function writes a single pixel to the x,y coords specified using the specified colour
 * Note colour is a byte and represents a palette number (0-255) not a 24 bit RGB value
 */
void write_a_pixel(int x, int y, int colour)
{
	WAIT_FOR_GRAPHICS;				// is graphics ready for new command

	GraphicsX1Reg = x;				// write coords to x1, y1
	GraphicsY1Reg = y;
	GraphicsColourReg = colour;			// set pixel colour
	GraphicsCommandReg = PutAPixel;			// give graphics "write pixel" command
}

/*
 * This function read a single pixel from the x,y coords specified and returns its colour
 * Note returned colour is a byte and represents a palette number (0-255) not a 24 bit RGB value
 */
int read_a_pixel(int x, int y)
{
	WAIT_FOR_GRAPHICS;			// is graphics ready for new command

	GraphicsX1Reg = x;			// write coords to x1, y1
	GraphicsY1Reg = y;
	GraphicsCommandReg = GetAPixel;		// give graphics a "get pixel" command

	WAIT_FOR_GRAPHICS;			// is graphics done reading pixel
	return (int)(GraphicsColourReg) ;	// return the palette number (colour)
}

/*
 * subroutine to program a hardware (graphics chip) palette number with an RGB value
 * e.g. program_palette(RED, 0x00FF0000) ;
 *
 */
void program_palette(int PaletteNumber, uint32_t RGB)
{
    WAIT_FOR_GRAPHICS;
    GraphicsColourReg = PaletteNumber;
    GraphicsX1Reg = RGB >> 16   ;        // program red value in ls.8 bit of X1 reg
    GraphicsY1Reg = RGB ;                // program green and blue into ls 16 bit of Y1 reg
    GraphicsCommandReg = ProgramPaletteColour; // issue command
}

/*
 * This function draw a horizontal line, 1 pixel at a time starting at the x,y coords specified
 */
void horizontal_line(int x1, int y1, int length, int colour)
{
	int x2 = x1 + length;
	WAIT_FOR_GRAPHICS;				// is graphics ready for new command
	GraphicsX1Reg = x1;				// write coords to x1, y1
	GraphicsY1Reg = y1;
	GraphicsX2Reg = x2;
	GraphicsY2Reg = y1;
	GraphicsColourReg = colour;			// set pixel colour
	GraphicsCommandReg = DrawHLine;			// give graphics "write pixel" command
}

/*
 *This function draw a vertical line, 1 pixel at a time starting at the x,y coords specified
 */
void vertical_line(int x1, int y1, int length, int colour)
{
	int y2 = y1 + length;
	WAIT_FOR_GRAPHICS;				// is graphics ready for new command
	GraphicsX1Reg = x1;				// write coords to x1, y1
	GraphicsY1Reg = y1;
	GraphicsX2Reg = x1;
	GraphicsY2Reg = y2;
	GraphicsColourReg = colour;			// set pixel colour
	GraphicsCommandReg = DrawVLine;			// give graphics "write pixel" command
}

/*
 * Implementation of Bresenhams line drawing algorithm
 */
void line(int x1, int y1, int x2, int y2, int colour)
{
	WAIT_FOR_GRAPHICS;				// is graphics ready for new command
	GraphicsX1Reg = x1;				// write coords to x1, y1
	GraphicsY1Reg = y1;
	GraphicsX2Reg = x2;
	GraphicsY2Reg = y2;
	GraphicsColourReg = colour;		// set pixel colour
	GraphicsCommandReg = DrawLine;	// give graphics "write pixel" command
}

/*
 * Draw a rectangle with top left corner x1,y1 and bottom right corner x2,y2, and with colour
 */
void rectangle(int x1, int y1, int x2, int y2, int colour)
{
	int i;

	for(i = 0; i < (y2-y1); i++){
		line(x1,y1+i,x2,y1+i,colour);
	}
}

/*
 * Draw a right pointing triangle with top edge at x1,y1 with height and colour
 */
void right_triangle(int x1, int y1, int height, int colour)
{
	int i;
	for(i=0;i<(height/2);i++){
		line(x1,y1+i,x1+i,y1+i,colour);
	}
	for(i=0;i<(height/2);i++){
		line(x1,y1+(height/2)+i-1,x1+(height/2)-i-1,y1+(height/2)+i-1,colour);
	}
}



/*
 * Draw a left pointing triangle with top edge at x1,y1 with height and colour
 */
void left_triangle(int x1, int y1, int height, int colour)
{
	int i;
	for(i=0;i<(height/2);i++){
		line(x1,y1+i,x1-i,y1+i,colour);
	}
	for(i=0;i<(height/2);i++){
		line(x1,y1+(height/2)+i-1,x1-(height/2)+i-1,y1+(height/2)+i-1,colour);
	}
}

/*
 * Draw a downwards pointing triangle with left edge at x1,y1 with height and colour
 */
void down_triangle(int x1, int y1, int height, int colour)
{
	int i;
	for(i=0;i<(height/2);i++){
		line(x1+i,y1,x1+i,y1+i,colour);
	}
	for(i=0;i<(height/2);i++){
		line(x1+(height/2)+i-1,y1,x1+(height/2)+i-1,y1+(height/2)-i-1,colour);
	}
}

/*
 * Draw an upwards pointing triangle with left edge at x1,y1 with height and colour
 */
void up_triangle(int x1, int y1, int height, int colour)
{
	int i;
	for(i=0;i<(height/2);i++){
		line(x1+i,y1,x1+i,y1-i,colour);
	}
	for(i=0;i<(height/2);i++){
		line(x1+(height/2)+i-1,y1,x1+(height/2)+i-1,y1-(height/2)+i-1,colour);
	}
}

/*
 * Draw circle centered at x0,y0 with radius
 */
void circle(int x0, int y0, int radius, int colour)

{
    int x = radius;
    int y = 0;
    int err = 0;

    while (x >= y)
    {
        write_a_pixel(x0 + x, y0 + y, colour);
        write_a_pixel(x0 + y, y0 + x, colour);
        write_a_pixel(x0 - y, y0 + x, colour);
        write_a_pixel(x0 - x, y0 + y, colour);
        write_a_pixel(x0 - x, y0 - y, colour);
        write_a_pixel(x0 - y, y0 - x, colour);
        write_a_pixel(x0 + y, y0 - x, colour);
        write_a_pixel(x0 + x, y0 - y, colour);

        if (err <= 0)
        {
            y += 1;
            err += 2*y + 1;
        }
        if (err > 0)
        {
            x -= 1;
            err -= 2*x + 1;
        }
    }
    fill(x0,y0,colour,colour);
}

/*
 * Clear the screen
 */
void clear_screen(void)
{
	int i;
	for(i=0;i<480;i++){
		line(0,i,799,i,BLACK);
	}
}

/*
 * Draw reticle to center of the screen
 */
void reticle(int colour)
{
	rectangle(398,150,402,330,colour);
	rectangle(310,238,490,242,colour);
	rectangle(340,272,460,274,colour);
	rectangle(370,304,430,306,colour);
}

void write_processing_message(int colour)
{
		OutGraphicsCharFont1(346,85,BLACK,colour,'P',0);
		OutGraphicsCharFont1(358,85,BLACK,colour,'R',0);
		OutGraphicsCharFont1(370,85,BLACK,colour,'O',0);
		OutGraphicsCharFont1(382,85,BLACK,colour,'C',0);
		OutGraphicsCharFont1(394,85,BLACK,colour,'E',0);
		OutGraphicsCharFont1(406,85,BLACK,colour,'S',0);
		OutGraphicsCharFont1(418,85,BLACK,colour,'S',0);
		OutGraphicsCharFont1(430,85,BLACK,colour,'I',0);
		OutGraphicsCharFont1(442,85,BLACK,colour,'N',0);
		OutGraphicsCharFont1(454,85,BLACK,colour,'G',0);


}

void erase_processing_message(void)
{
	rectangle(345,60,460,80,BLACK);
}

/*
 * Draw set arrow layout with colour
 */
void print_display(int arrow_colour, int menu_colour, int screen_colour,
				  int reticle_colour, int fire_colour, int camera_colour)
{
	clear_screen();
	//rectangle(60,60,740,420,screen_colour);
	//reticle(reticle_colour);

	//MENU ICONS
	draw_button(fire,fire_colour);
	draw_button(camera,camera_colour)
	draw_button(security,menu_colour);
	draw_button(manual,menu_colour);
	draw_button(automatic,menu_colour);
	draw_button(left,arrow_colour);
	draw_button(right,arrow_colour);
	draw_button(up,arrow_colour);
	draw_button(down,arrow_colour);
}

void draw_button(button_type button, int colour){
	switch(button){

	case(fire):
		circle(600,440,30,colour);
		OutGraphicsCharFont1(585,437,BLACK,colour,'F',0);
		OutGraphicsCharFont1(595,437,BLACK,colour,'I',0);
		OutGraphicsCharFont1(605,437,BLACK,colour,'R',0);
		OutGraphicsCharFont1(615,437,BLACK,colour,'E',0);
		break;

	case(camera):
		circle(200,440,30,colour);
		rectangle(180,425,220,455,GREY);
		circle(200,440,10,BLACK);
		circle(200,440,8,WHITE);
		rectangle(185,420,195,425,BLACK);
		circle(215,430,2,RED);
		break;
	case(manual):
		rectangle(500,10,580,50,colour);
		OutGraphicsCharFont1(520,27,BLACK,colour,'M',0);
		OutGraphicsCharFont1(528,27,BLACK,colour,'A',0);
		OutGraphicsCharFont1(536,27,BLACK,colour,'N',0);
		OutGraphicsCharFont1(544,27,BLACK,colour,'U',0);
		OutGraphicsCharFont1(552,27,BLACK,colour,'A',0);
		OutGraphicsCharFont1(560,27,BLACK,colour,'L',0);
		break;

	case(automatic):
		rectangle(590,10,670,50,colour);
		OutGraphicsCharFont1(618,27,BLACK,colour,'A',0);
		OutGraphicsCharFont1(624,27,BLACK,colour,'U',0);
		OutGraphicsCharFont1(632,27,BLACK,colour,'T',0);
		OutGraphicsCharFont1(640,27,BLACK,colour,'O',0);
		break;

	case(security):
		rectangle(680,10,760,50,colour);
		OutGraphicsCharFont1(692,27,BLACK,colour,'S',0);
		OutGraphicsCharFont1(700,27,BLACK,colour,'E',0);
		OutGraphicsCharFont1(708,27,BLACK,colour,'C',0);
		OutGraphicsCharFont1(716,27,BLACK,colour,'U',0);
		OutGraphicsCharFont1(724,27,BLACK,colour,'R',0);
		OutGraphicsCharFont1(732,27,BLACK,colour,'I',0);
		OutGraphicsCharFont1(740,27,BLACK,colour,'T',0);
		OutGraphicsCharFont1(748,27,BLACK,colour,'Y',0);
		break;

	case(left):
		left_triangle(LARROW_X1,LARROW_Y1,ARROW_LENGTH,colour);
		break;

	case(right):
		right_triangle(RARROW_X1,RARROW_Y1,ARROW_LENGTH,colour);
		break;

	case(down):
		down_triangle(DARROW_X1,DARROW_Y1,ARROW_LENGTH,colour);
		break;

	case(up):
		up_triangle(UARROW_X1,UARROW_Y1,ARROW_LENGTH,colour);
		break;

	case(fire_pressed):
			circle(600,450,17,colour);
			OutGraphicsCharFont1(590,447,BLACK,colour,'F',0);
			OutGraphicsCharFont1(596,447,BLACK,colour,'I',0);
			OutGraphicsCharFont1(602,447,BLACK,colour,'R',0);
			OutGraphicsCharFont1(608,447,BLACK,colour,'E',0);
			break;

	case(manual_pressed):
			rectangle(503,13,577,47,colour);
			OutGraphicsCharFont1(520,27,BLACK,colour,'M',0);
			OutGraphicsCharFont1(528,27,BLACK,colour,'A',0);
			OutGraphicsCharFont1(536,27,BLACK,colour,'N',0);
			OutGraphicsCharFont1(544,27,BLACK,colour,'U',0);
			OutGraphicsCharFont1(552,27,BLACK,colour,'A',0);
			OutGraphicsCharFont1(560,27,BLACK,colour,'L',0);
			break;

	case(automatic_pressed):
			rectangle(593,13,667,47,colour);
			OutGraphicsCharFont1(618,27,BLACK,colour,'A',0);
			OutGraphicsCharFont1(624,27,BLACK,colour,'U',0);
			OutGraphicsCharFont1(632,27,BLACK,colour,'T',0);
			OutGraphicsCharFont1(640,27,BLACK,colour,'O',0);
			break;

	case(security_pressed):
			rectangle(683,13,757,47,colour);
			OutGraphicsCharFont1(692,27,BLACK,colour,'S',0);
			OutGraphicsCharFont1(700,27,BLACK,colour,'E',0);
			OutGraphicsCharFont1(708,27,BLACK,colour,'C',0);
			OutGraphicsCharFont1(716,27,BLACK,colour,'U',0);
			OutGraphicsCharFont1(724,27,BLACK,colour,'R',0);
			OutGraphicsCharFont1(732,27,BLACK,colour,'I',0);
			OutGraphicsCharFont1(740,27,BLACK,colour,'T',0);
			OutGraphicsCharFont1(748,27,BLACK,colour,'Y',0);
			break;

	case(up_pressed):
		up_triangle(UARROW_X1 + 12,UARROW_Y1-6,ARROW_LENGTH - 24,colour);
		break;

	case(down_pressed):
		down_triangle(DARROW_X1 + 12,DARROW_Y1+6,ARROW_LENGTH - 24,colour);
		break;

	case(left_pressed):
		left_triangle(LARROW_X1 - 6,LARROW_Y1+12,ARROW_LENGTH - 24,colour);
		break;

	case(right_pressed):
		right_triangle(RARROW_X1 + 6,RARROW_Y1+12,ARROW_LENGTH - 24,colour);
		break;
	}
}


// data structure to store a coordinate

typedef struct { int x,y;} XYPixel ;

// an array of coordinates and a pointer to first

XYPixel XYStack[1000], *Next = &XYStack[0];


/*****************************************************************************************************************************
* simple stack based around an array
* Used for FILL algorithm
******************************************************************************************************************************/

int push_pixel(XYPixel p1)
{
    if(Next <= &XYStack[1000]) {
        *Next++ = p1;
        return 0 ;
    }
    else
        return -1 ;
}

int pop_pixel(XYPixel *theXYPixel)
{
    if(Next >= XYStack) {
        *theXYPixel = *(--Next);
        return 0 ;
    }
    else
        return -1 ;
}

int is_stack_empty(void)
{
    if(Next == XYStack)
        return 1 ;
    else
        return 0 ;
}

/********************************************************************************************
** fill Algorithm
** Taken from Procedural Elements of Computer Graphics: David Rogers
********************************************************************************************/
void fill(int _x, int _y, int _FillColour, int _BoundaryColour)
{
    register int     x, y ;
    register int     BoundaryColour = _BoundaryColour;
    register int 	 PixelColour, FillColour = _FillColour ;

    int     XRight, XLeft ;
    int     SaveX, SaveY ;      		// temp variable
    XYPixel aPoint, aPoint1 ;           // temp var

    Next = XYStack ;                    // initialise to start of stack
    aPoint.x = _x ;
    aPoint.y = _y ;

    push_pixel(aPoint) ;                   // push the seed

    while(!is_stack_empty())                 // keep going until no more items on the stack
    {
        pop_pixel(&aPoint) ;                 // get a point from the stack
        x = aPoint.x ;
        y = aPoint.y ;
        write_a_pixel(x, y, FillColour);     // fill the point in the fill colour

        // fill the span to the right of the seed value
        SaveX = x++ ;                  // save the x coord of the the point we just filled and move one pixel right

        while((char)(read_a_pixel(x,y)) != (char)(BoundaryColour))							// if new pixel is not the boundary colour
            write_a_pixel(x++, y, FillColour);     											// fill it and keep moving right along a horizontal line

        // must have found the boundary colour when moving right
        XRight = x - 1 ;		// save X coord of the last filled pixel on this line when moving right
        x = SaveX ;				// get the original starting x back

        // now fill the span to the left of the seed value

        --x ;

        while((char)(read_a_pixel(x,y)) != (char)(BoundaryColour))						// if new pixel is not the boundary colour
            write_a_pixel(x--, y, FillColour);    											// fill it and keep moving left along a horizontal line

        XLeft = x + 1 ;			// save X coord of the last filled pixel on this line when moving left
        x = SaveX ; 			// get original x coord for the seed back

		///////////////////////////////////////////////////////////////////////////////////////////////////
        // check that the scan line below is neither a polygon boundary nor
        // has been previously completely filled
        //////////////////////////////////////////////////////////////////////////////////////////////////

        SaveY = y ;			// save the current y coordinate of the line we have just drawn
        x = XLeft ;			// starting at the left x
        ++y ;				// move down one line

		// starting from the left keep moving right looking at the pixel
        // until we find something that is neither filled nor boundary colour as it represents something on the line that may be a pixel to fill

        do {
        	PixelColour = read_a_pixel(x++,y) ;
        } while(((char)(PixelColour) == (char)(BoundaryColour)) || ((char)(PixelColour) == (char)(FillColour)) );

         x-- ;

        // to get here we must have found something that needs filling i.e. the above loop found that the line below was not a complete boundary edge or filled
		// if we are still less than the previous right most X coord then it would be a new point that we need to seed
        while(x <= XRight)
        {
            // seed the scan line below
        	// if the pixel at x,y is not a boundary colour and less than extreme right

        	// skip over any pixels already filled
            while(((char)(read_a_pixel(x,y)) != (char)(BoundaryColour)) && (x <= XRight))
               ++x ;

            // push the  extreme right pixel onto the stack
            aPoint1.x = x - 1 ;
            aPoint1.y = y ;
            push_pixel(aPoint1) ;

            // continue checking in case the span is interrupted by another shape inside the one we are trying to fill

            ++x ;

            // skip over anything that is filled or boundary (i.e. other shape) inside the one we are trying to fill
            do {
            	PixelColour = read_a_pixel(x++,y) ;
            } while(((char)(PixelColour) == (char)(BoundaryColour)) || ((char)(PixelColour) == (char)(FillColour)) );

             x-- ;
        }
      	x = SaveX ;
       	y = SaveY ;

	 ///////////////////////////////////////////////////////////////////////////////////////////////////
    // check that the scan line above is neither a polygon boundary nor
    // has been previously completely filled

        y = SaveY;
        x = XLeft ;
        --y ;

        do {
        	PixelColour = read_a_pixel(x++,y) ;
        } while(((char)(PixelColour) == (char)(BoundaryColour)) || ((char)(PixelColour) == (char)(FillColour)) );

         x-- ;

        while(x <= XRight)		// if we have not reached the boundary
        {
            // seed the scan line below
            while(((char)(read_a_pixel(x,y)) != (char)(BoundaryColour)) && (x <= XRight))
               ++x ;		// look for right most x inside the boudan

            // push the  extreme right pixel onto the stack
            aPoint1.x = x - 1 ;
            aPoint1.y = y ;
            push_pixel(aPoint1) ;

            // continue checking in case the span is interrupted
            ++x ;

            do {
            	PixelColour = read_a_pixel(x++,y) ;
            } while(((char)(PixelColour) == (char)(BoundaryColour)) || ((char)(PixelColour) == (char)(FillColour)) );

             x-- ;
        }
       	x = SaveX ;
       	y = SaveY ;
    }
}
/*************************************************************************************************
** This function draws a single ASCII character at the coord and colour specified
** it optionally ERASES the background colour pixels to the background colour
** This means you can use this to erase characters
**
** e.g. writing a space character with Erase set to true will set all pixels in the
** character to the background colour
**
*************************************************************************************************/
void OutGraphicsCharFont1(int x, int y, int fontcolour, int backgroundcolour, int c, int Erase)
{
	// using register variables (as opposed to stack based ones) may make execution faster
	// depends on compiler and CPU
	register int row, column, theX = x, theY = y ;
	register int pixels ;
	register char theColour = fontcolour ;
	register int BitMask, theC = c ;
	// if x,y coord off edge of screen don't bother
	// XRES and YRES are #defined to be 800 and 480 respectively
	 if(((short)(x) > (short)(XRES-1)) || ((short)(y) > (short)(YRES-1)))
	 return ;
	// if printable character subtract hex 20
	if(((short)(theC) >= (short)(' ')) && ((short)(theC) <= (short)('~'))) {
		theC = theC - 0x20 ;
		for(row = 0; (char)(row) < (char)(7); row ++) {
			// get the bit pattern for row 0 of the character from the software font
			pixels = Font5x7[theC][row] ;
			BitMask = 16 ;
			for(column = 0; (char)(column) < (char)(5); column ++) {
			// if a pixel in the character display it
			if((pixels & BitMask))
			write_a_pixel(theX+column, theY+row, theColour) ;
			else {
				if(Erase == 1)
				// if pixel is part of background (not part of character)
				// erase the background to value of variable BackGroundColour
				write_a_pixel(theX+column, theY+row, backgroundcolour) ;
			}
			BitMask = BitMask >> 1 ;
		}
		}
	}
}


/******************************************************************************************************************************
** This function draws a single ASCII character at the coord specified using the colour specified
** OutGraphicsCharFont2(100,100, RED, 'A', TRUE, FALSE, 1, 1) ;	// display upper case 'A' in RED at coords 100,100, erase background
** no scroll, scale x,y= 1,1
**
******************************************************************************************************************************/
void OutGraphicsCharFont2a(int x, int y, int colour, int backgroundcolour, int c, int Erase)
{
	register int 	row,
					column,
					theX = x,
					theY = y ;
	register int 	pixels ;
	register char 	theColour = colour  ;
	register int 	BitMask,
					theCharacter = c,
					j,
					theRow, theColumn;


    if(((short)(x) > (short)(XRES-1)) || ((short)(y) > (short)(YRES-1)))  // if start off edge of screen don't bother
        return ;

	if(((short)(theCharacter) >= (short)(' ')) && ((short)(theCharacter) <= (short)('~'))) {			// if printable character
		theCharacter -= 0x20 ;																			// subtract hex 20 to get index of first printable character (the space character)
		theRow = FONT2_YPIXELS;
		theColumn = FONT2_XPIXELS;

		for(row = 0; row < theRow ; row ++)	{
			pixels = Font10x14[theCharacter][row] ;		     								// get the pixels for row 0 of the character to be displayed
			BitMask = 512 ;							   											// set of hex 200 i.e. bit 7-0 = 0010 0000 0000
			for(column = 0; column < theColumn;   )  	{
				if((pixels & BitMask))														// if valid pixel, then write it
					write_a_pixel(theX+column, theY+row, theColour) ;
				else {																		// if not a valid pixel, do we erase or leave it along (no erase)
					if(Erase == 1)
						write_a_pixel(theX+column, theY+row, backgroundcolour) ;
					// else leave it alone
				}
					column ++ ;
				BitMask = BitMask >> 1 ;
			}
		}
	}
}

void init_palette()
{
	int i,j,k;
	int index;
	uint32_t value;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			for (k = 0; k < 4; k++){
				index = 16*i + 4*j + k;
				value = 5570560*i + 21760*j + 85*k;
				program_palette(index, value);
			}
}

void print_image(unsigned char arr[320][240], int x_size, int y_size)
{
    int i,j;

    for (j = 0; j < y_size; j++)
    {
        for (i = 0; i < x_size; i++)
        {
        write_a_pixel(PICTURE_X_OFFSET + i, PICTURE_Y_OFFSET + j, arr[i][j]);
        }
    }
}


