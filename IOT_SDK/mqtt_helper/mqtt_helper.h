/*
 * mqtt_helper.h
 */

#ifndef MQTT_WRAPPER_H
#define MQTT_WRAPPER_H

#include <stdint.h>
#include <stddef.h>
#include "esp32_at.h"
#include "application_config.h"

typedef enum {
    MQTT_OPERATION_PUBLISH,
    MQTT_OPERATION_SUBSCRIBE,
    MQTT_OPERATION_RECEIVE
} mqtt_operation_t;

typedef struct {
    mqtt_operation_t operation;
    char topic[MAX_MQTT_TOPIC_SIZE];
    size_t topic_length;
    uint8_t payload[MAX_MQTT_PAYLOAD_SIZE];
    size_t payload_length;
} mqtt_queue_item_t;

mqtt_status_t mqtt_connect(char *thingname, char *host, uint16_t port);
mqtt_status_t mqtt_publish(char *topic, size_t topic_length, uint8_t *message, size_t message_length);
mqtt_status_t mqtt_subscribe(char *topic, size_t topic_length);

#endif
