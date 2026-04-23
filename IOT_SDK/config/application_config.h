/*
 * application_config.h
 *
 *  Created on: Mar 28, 2025
 *      Author: Shreyas Acharya, BHARATI SOFTWARE
 */

#ifndef INC_APPLICATION_CONFIG_H_
#define INC_APPLICATION_CONFIG_H_


/**************************************************/
/******* DO NOT CHANGE the following order ********/
/**************************************************/

/* Include logging header files and define logging macros in the following order:
 * 1. Include the header file "logging_levels.h".
 * 2. Define the LIBRARY_LOG_NAME and LIBRARY_LOG_LEVEL macros depending on
 * the logging configuration.
 * 3. Include the header file "logging_stack.h", if logging is enabled.
 */

#include "logging_levels.h"

#ifndef LIBRARY_LOG_NAME
    #define LIBRARY_LOG_NAME               "MAIN_APP"
#endif

#ifndef LIBRARY_LOG_LEVEL
    #define LIBRARY_LOG_LEVEL              LOG_INFO
#endif

#include "logging_stack.h"

#define WIFI_SSID                          "YOUR_WIFI_SSID"
#define WIFI_PASSWORD                      "YOUR_WIFI_PASSWORD"

#define MQTT_BROKER                        "your-endpoint.iot.us-east-1.amazonaws.com"
#define MQTT_PORT                          8883

#define CLIENT_ID                          "my_iot_device_1"

#define UTC_OFFSET                         -400

#define SENSOR_DATA_TOPIC                  "sensors/room1/temp_humidity"

/*#define _TOPIC                            "/payments"*/

#define MQTT_PUBLISH_TIME_BETWEEN_MS       ( 10000U )

#define MAX_MQTT_PAYLOAD_SIZE              512
#define MAX_MQTT_TOPIC_SIZE                80


#endif /* INC_APPLICATION_CONFIG_H_ */
