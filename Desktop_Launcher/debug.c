/*
 * debug.c
 *
 *  Created on: Jan 27, 2017
 *      Author: Zeyad
 */


void print_byte_array(unsigned char * byte_array, int size)
{
    int byte_array_index;
    for (byte_array_index = 0; byte_array_index < size; byte_array_index++)
        printf("[%02x]", byte_array[byte_array_index]);
    printf("\n");
}
