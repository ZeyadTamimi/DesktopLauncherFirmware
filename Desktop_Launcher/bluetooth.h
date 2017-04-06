#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

//===================================================================
// Includes
//===================================================================
#include<stdint.h>

//===================================================================
// Message IDs
//===================================================================
// Commands
#define ID_COMMAND_MOVE_TIME 0x01
#define ID_COMMAND_MOVE_ANGLE 0x02
#define ID_COMMAND_CHANGE_SPEED 0x03
#define ID_COMMAND_FIRE 0x04
#define ID_COMMAND_MOVE_TIME_SPEED 0x05
#define ID_COMMAND_CHANGE_RES 0x06
#define ID_RESPONSE 0xAA
#define ID_REQUEST 0xF1
#define ID_MESG_IMAGE 0xF2
#define ID_MESG_MOTION 0xF3
#define ID_ANDROID_HANDSHAKE 0xFF

// Field Sizes
#define SIZE_FIELD_ID 1
#define SIZE_FIELD_LENGTH 2
#define SIZE_FIELD_HEADER (SIZE_FIELD_ID + SIZE_FIELD_LENGTH)
#define SIZE_FIELD_COMMAND_MOVE_DIR 1
#define SIZE_FIELD_COMMAND_MOVE_TIME 4
#define SIZE_FIELD_COMMAND_MOVE_SPEED 1
#define SIZE_FIELD_COMMAND_MOVE_ANGLE_Y 1
#define SIZE_FIELD_COMMAND_MOVE_ANGLE_X 1

// Response Codes
#define RESPONSE_NO_ERROR 0
#define RESPONSE_INVALID_PARAM 1
#define RESPONSE_INVALID_COMMAND 2
#define RESPONSE_INVALID_REQUEST 3
#define RESPONSE_NIOS_HANDSHAKE 4

//===================================================================
// Public Function Declaration
//===================================================================
void init_bluetooth(void);
int bluetooth_receive_message_timeout(uint8_t **message_buffer, unsigned long timeout);
void bluetooth_send_response(uint8_t response_message_id, uint8_t response_code);
void bluetooth_send_image(uint8_t *image_data, uint16_t image_size);
void bluetooth_send_motion(uint8_t detected);
int bluetooth_connected(void);
void flush_buffer(void);

#endif /* BLUETOOTH_H_ */
