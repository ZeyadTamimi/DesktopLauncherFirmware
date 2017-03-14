#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#include<stdint.h>

// Message IDs
#define MOVE_COMMAND_TIME_ID 0x01
#define MOVE_COMMAND_ANGLE_ID 0x02
#define MOVE_COMMAND_CHANGE_SPEED 0x03
#define IMAGE_REQUEST 0x10
#define RESPONSE_ID 0xF1
#define IMAGE_ID 0xF2

// Field Sizes
#define MESG_FIELD_ID_SIZE 1
#define MESG_FIELD_LENGTH_SIZE 2
#define MESG_FIELD_HEADER_SIZE MESG_FIELD_ID_SIZE+MESG_FIELD_LENGTH_SIZE
#define MESG_MOVE_FIELD_DIR_SIZE 1
// Response IDs
#define RESPONSE_NO_ERROR 0
#define RESPONSE_INVALID_PARAM 1

// Message Lengths
#define MOVE_COMMAND_TIME_LENGTH 4

#define BUFFER_SIZE 256
unsigned char in_buffer[BUFFER_SIZE];

void bluetooth_main(void);
void init_bluetooth(void);
int bluetooth_receive_message_timeout(uint8_t **message_buffer, unsigned long timeout);
void bluetooth_send_response(uint8_t response_message_id, uint8_t response_code);
void bluetooth_send_image(uint8_t *image_data, uint16_t image_size);
int bluetooth_connected(void);


#endif /* BLUETOOTH_H_ */
