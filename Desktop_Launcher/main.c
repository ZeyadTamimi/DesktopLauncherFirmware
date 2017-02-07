#include "gui.h"
#include "touch_screen.h"
#include "motor.h"
#include "nano_jpeg.h"
#include "camera.h"
#include "graphics.h"


#define _NJ_INCLUDE_HEADER_ONLY

int cam_unit_test(void)
{
    njInit();
    init_palette();
    camera_reset();


    cam_init();
    unsigned char y = get_image_resolution();
    printf("Data is: [%02x]\n", y);
    y = set_image_resolution(FRAME_640x480);
    printf("Set returned : [%02x]\n", y);
    y = get_image_resolution();
    printf("Data is: [%02x]\n", y);

    take_picture();
    uint32_t size = frame_length();
    uint32_t len = size;
    printf("Frame size: %ld\n", size);

//  printf("start");
//  FILE *fp;
//  fp = fopen("test", 'w');
//  fprintf(fp, "This is testing for fprintf...\n");
//  fputs("This is testing for fputs...\n", fp);
//  fclose(fp);
//  printf("done");

    int current_index = 0;
    int i;
    uint8_t *jpeg_buffer = malloc(sizeof(uint8_t)*size);
    uint8_t *buffer;

    while (size > 64) {
        // read 32 bytes at a time;
        buffer = read_picture(64);
        size -= 64;
        for(i=0;i<64;i++)
            jpeg_buffer[current_index+i] = *(buffer+i);
        current_index += 64;
    }

    buffer = read_picture(size);
    for(i=0;i<size;i++)
        jpeg_buffer[current_index+i] = *(buffer+i);


    printf("\nStarted Decoding\n");

    //print_byte_array(jpeg_buffer, len);
    printf("The value of decode is: %d\n", njDecode(jpeg_buffer,len));
    printf("The value of width is: %d\n", njGetWidth());
    printf("The value of height is: %d\n", njGetHeight());
    printf("The value of size is: %d\n", njGetImageSize());

    printf("Copying into array\n");

    unsigned char *bitmap = njGetImage();
    unsigned char PMF[640][480];
    unsigned char value;
    int curr;
    for(curr = 0; curr < njGetImageSize()/3; curr++)
    {
        value = (bitmap[3*curr] & 0xC0) | (bitmap[3*curr+1] & 0xC0)>>2 |  (bitmap[3*curr+2] & 0xC0)>>4;
        int R = bitmap[3*curr];
        int G = bitmap[3*curr+1];
        int B = bitmap[3*curr+2];
        PMF[curr%640][(int)(curr/640)] = (value)>>2;
    }

    print_image(PMF, 640, 480);
    njDone();
    return 0;
}

int main()
{
    //cam_unit_test();
    njInit();
    cam_init();
    camera_reset();
    unsigned char y = get_image_resolution();
    printf("Data is: [%02x]\n", y);
    y = set_image_resolution(FRAME_320x240);
    printf("Set returned : [%02x]\n", y);
    y = get_image_resolution();
    printf("Data is: [%02x]\n", y);
    init_gui();

    // Getting an image setup
    while (1)
        proccess_buttons(10000);
    printf("done\n");
    njDone();
    return 0;
}


