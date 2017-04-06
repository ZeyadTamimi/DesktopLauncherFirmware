#ifndef WIFI_H_
#define WIFI_H_

//===================================================================
// Includes
//===================================================================
// System Includes
#include <stdint.h>
#include <stdlib.h>

//===================================================================
// Defines
//===================================================================
#define MOTION_DETECTED 0

//===================================================================
// Public Function Declarations
//===================================================================
void init_wifi(void);
void send_sms(const char* body, size_t len);
void connect_db(void);
void close_db_connection(void);

#endif /* WIFI_H_ */
