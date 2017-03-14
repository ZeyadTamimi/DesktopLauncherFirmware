#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#include<stdint.h>

// Message IDs
#define MOVE_COMMAND_TIME_ID 0x01
#define MOVE_COMMAND_ANGLE_ID 0x02
#define MOVE_COMMAND_CHANGE_SPEED 0x03
#define IMAGE_REQUEST 0x10

// FIeld Sizes
#define MESG_FIELD_ID_SIZE 1
#define MESG_FIELD_LENGTH_SIZE 2
#define MESG_FIELD_HEADER_SIZE MESG_FIELD_ID_SIZE+MESG_FIELD_LENGTH_SIZE
#define MESG_MOVE_FIELD_DIR_SIZE 1
// Response IDs
#define RESPONSE_NO_ERROR 0
#define RESPONSE_INVALID_PARAM 1

// Message Lengths
#define MOVE_COMMAND_TIME_LENGTH 4

void init_bluetooth(void);
int bluetooth_receive_message_timeout(uint8_t message_buffer, unsigned long timeout);
int bluetooth_send_response(uint8_t response_message, uint8_t response_code);


#endif /* BLUETOOTH_H_ */
