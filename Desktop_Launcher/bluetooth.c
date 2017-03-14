//===================================================================
// Includes
//===================================================================
#include <stdint.h>

#include "serial.h"
#include "bluetooth.h"
//===================================================================
// Defines
//===================================================================


//===================================================================
// Public Function Definitions
//===================================================================
void init_bluetooth(void)
{
	init_serial(BLUETOOTH);
}

int bluetooth_receive_message_timeout(uint8_t message_buffer, unsigned long timeout)
{
	return 0;
}

int bluetooth_send_response(uint8_t response_message, uint8_t response_code)
{
	return 0;
}
