#ifndef DHT11_H_
#define DHT11_H_

#include "main.h"
#include <stdint.h>

typedef enum {
    DHT11_OK = 0,
    DHT11_ERROR = 1,
    DHT11_TIMEOUT = 2,
    DHT11_CHECKSUM_ERROR = 3
} dht11_status_t;

typedef struct {
    uint8_t humidity;
    float temperature;
} dht11_data_t;

void dht11_init(void);
dht11_status_t dht11_read(dht11_data_t *data);

#endif /* DHT11_H_ */
