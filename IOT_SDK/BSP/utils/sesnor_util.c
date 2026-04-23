#include "sensor_util.h"
#include <string.h>
#include <stdio.h>
#include "dht11.h"

static dht11_data_t last_reading = {0};

int init_temperature_humidity_sensor(void) {
    LogInfo(("Initializing DHT11 temperature and humidity sensor..."));
    dht11_init();
    LogInfo(("DHT11 sensor initialization complete."));
    return 0;
}

void get_temperature_reading(float *temperature) {
    if (temperature == NULL) {
        LogError(("Invalid NULL pointer for temperature read."));
        return;
    }

    dht11_status_t status = dht11_read(&last_reading);
    if (status == DHT11_OK) {
        *temperature = last_reading.temperature;
    } else {
        *temperature = 0.0f;
    }
    LogInfo(("Temperature: %d C", (int)*temperature));
}

void get_humidity_reading(uint8_t *humidity) {
    if (humidity == NULL) {
        LogError(("Invalid NULL pointer for humidity read."));
        return;
    }

    /* Use data from the last dht11_read call in get_temperature_reading */
    *humidity = last_reading.humidity;
    LogInfo(("Humidity: %u%%", *humidity));
}
