#include "dht11.h"
#include <string.h>

/* Microsecond delay using DWT cycle counter */
static void delay_us(uint32_t us) {
    uint32_t start = DWT->CYCCNT;
    uint32_t ticks = us * (SystemCoreClock / 1000000);
    while ((DWT->CYCCNT - start) < ticks);
}

static void dht11_set_pin_output(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DHT11_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStruct);
}

static void dht11_set_pin_input(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DHT11_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStruct);
}

void dht11_init(void) {
    /* Enable DWT cycle counter for microsecond timing */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

    /* Enable GPIO clock for port A */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* Set pin high (idle state) */
    dht11_set_pin_output();
    HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_SET);
}

dht11_status_t dht11_read(dht11_data_t *data) {
    uint8_t raw[5] = {0};
    uint32_t timeout;

    if (data == NULL) return DHT11_ERROR;

    /* Step 1: Send start signal - pull low for 20ms */
    dht11_set_pin_output();
    HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_RESET);
    HAL_Delay(20);
    HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_SET);
    delay_us(30);

    /* Step 2: Switch to input and wait for DHT11 response */
    dht11_set_pin_input();

    /* Wait for DHT11 to pull low (response signal) */
    timeout = 100;
    while (HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin) == GPIO_PIN_SET) {
        delay_us(1);
        if (--timeout == 0) return DHT11_TIMEOUT;
    }

    /* DHT11 holds low for ~80us */
    timeout = 100;
    while (HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin) == GPIO_PIN_RESET) {
        delay_us(1);
        if (--timeout == 0) return DHT11_TIMEOUT;
    }

    /* DHT11 holds high for ~80us */
    timeout = 100;
    while (HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin) == GPIO_PIN_SET) {
        delay_us(1);
        if (--timeout == 0) return DHT11_TIMEOUT;
    }

    /* Step 3: Read 40 bits (5 bytes) of data */
    for (int i = 0; i < 40; i++) {
        /* Wait for low period (~50us) */
        timeout = 100;
        while (HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin) == GPIO_PIN_RESET) {
            delay_us(1);
            if (--timeout == 0) return DHT11_TIMEOUT;
        }

        /* Measure high period duration */
        delay_us(40);

        /* If still high after 40us, it's a '1'; otherwise '0' */
        if (HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin) == GPIO_PIN_SET) {
            raw[i / 8] |= (1 << (7 - (i % 8)));

            /* Wait for pin to go low before next bit */
            timeout = 100;
            while (HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin) == GPIO_PIN_SET) {
                delay_us(1);
                if (--timeout == 0) return DHT11_TIMEOUT;
            }
        }
    }

    /* Step 4: Return to idle - set pin high */
    dht11_set_pin_output();
    HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_SET);

    /* Step 5: Verify checksum */
    uint8_t checksum = raw[0] + raw[1] + raw[2] + raw[3];
    if (checksum != raw[4]) {
        return DHT11_CHECKSUM_ERROR;
    }

    /* raw[0] = humidity integer, raw[1] = humidity decimal (always 0 for DHT11) */
    /* raw[2] = temperature integer, raw[3] = temperature decimal */
    data->humidity = raw[0];
    data->temperature = (float)raw[2] + (float)raw[3] * 0.1f;

    return DHT11_OK;
}
