#include "gui.h"
#include "touch_screen.h"
#include "motor.h"
#include "nano_jpeg.h"
#include "camera.h"
#include "graphics.h"

#define _NJ_INCLUDE_HEADER_ONLY

#define SECURITY_POLL 1000000
#define USER_POLL 1000000



typedef enum desktop_launcher_mode
{
	MANUAL= 0x00,
	SECURITY = 0x01
}desktop_launcher_mode;

desktop_launcher_mode current_mode;
desktop_launcher_mode past_mode;


void manual_callback()
{
	desktop_launcher_mode = MANUAL;
}


void security_callback()
{
	desktop_launcher_mode = SECURITY;
}

/*void fire_callback()
{

}

void left_callback()
{

}
void right_callback()
{

}
void up_callback()
{

}
void down_callback()
{

}*/

void retrieve_and_process_image(void)
{
    take_picture();
    uint32_t size = frame_length();
    uint32_t len = size;
    printf("Frame size: %ld\n", size);

    int current_index = 0;
    int i;
    uint8_t *jpeg_buffer = malloc((sizeof(uint8_t)*size) + 10);
    uint8_t *buffer;

    /*while (size > 64) {
        // read 32 bytes at a time;
        buffer = read_picture(64);
        size -= 64;
        for(i=0;i<64;i++)
            jpeg_buffer[current_index+i] = *(buffer+i);
        current_index += 64;
    }

    buffer = read_picture(size);
    for(i=0;i<size;i++)
        jpeg_buffer[current_index+i] = *(buffer+i);*/

    while (size > 64) {
            // read 32 bytes at a time;
            read_picture_to_ptr(jpeg_buffer, 64);
            size -= 64;
        }

    read_picture_to_ptr(jpeg_buffer, size);


    resume_picture();
    printf("DECODING\n");

    //print_byte_array(jpeg_buffer, len);
    printf("The value of decode is: %d\n", njDecode(jpeg_buffer,len));


    unsigned char *bitmap = njGetImage();
    unsigned char PMF[320][240];
    unsigned char value;
    int curr;
    for(curr = 0; curr < njGetImageSize()/3; curr++)
    {
        value = (bitmap[3*curr] & 0xC0) | (bitmap[3*curr+1] & 0xC0)>>2 |  (bitmap[3*curr+2] & 0xC0)>>4;
        PMF[curr%320][(int)(curr/320)] = (value)>>2;
    }

    print_image(PMF, 320, 240);
    free(jpeg_buffer);
}


/*int void_main()
{
    njInit();
    cam_init();
    init_gui();


    camera_reset();
    unsigned char y = get_image_resolution();
    printf("Data is: [%02x]\n", y);
    y = set_image_resolution(FRAME_320x240);
    printf("Set returned : [%02x]\n", y);
    y = get_image_resolution();
    printf("Data is: [%02x]\n", y);

    // Getting an image setup
    while (1)
        proccess_buttons(10000);
    printf("done\n");
    njDone();
    return 0;
}*/


void security_mode(void)
{
	/**********************
	 * CODE TO DISABLE BUTTONS/CALLBACKS
	 * SEND SMS WITH TIME
	 * SAVE PHOTO TO CLOUD
	 **********************/
	uint8_t * jpeg_photo_buffer;
	set_motion_detect(1);
	if (get_motion_detect())
		printf("ON\n");
	  else
		printf("OFF\n");

	set_motion_detect(1);
	if (motion_detected(SECURITY_POLL))
	{
	    if (take_picture())
	    {
	    	printf("TOOK THE PHOTO!\n");
	    	uint32_t photo_size = read_full_picture(jpeg_photo_buffer);
	    	/**********************
			 * WIFI CODE HERE
			 * SEND SMS WITH TIME
			 * SAVE PHOTO TO CLOUD
			 **********************/
	    }

	}

	free(jpeg_photo_buffer);
	resume_picture();
	set_motion_detect(0);
}

int main (void)
{
	cam_init();
    camera_reset();
    printf(" Image Resolution was %d\n", get_image_resolution());
    set_image_resolution(FRAME_160x120);
	printf(" Image Resolution was %d\n", get_image_resolution());
    /**********************
     * MOTOR INIT CODE HERE
     **********************/
	init_gui();
	/**********************
	 * CALLBACK INIT CODE HERE
	 **********************/


	current_mode = MANUAL;
	past_mode = MANUAL;


    while (1)
    {
    	process_user_input(USER_POLL);
    	switch (current_mode)
    	{
    	case SECURITY:
    		security_mode();
    	}
    }
}


