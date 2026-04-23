# STM32 AWS IoT Core — Secure MQTT Sensor Node

A bare-metal embedded IoT system that reads environmental sensor data on an STM32F446RE microcontroller, transmits it securely over TLS-encrypted MQTT to AWS IoT Core via an ESP32 Wi-Fi bridge, and receives cloud commands in real time.

## System Overview

┌──────────────┐    UART4     ┌──────────────┐   TLS/MQTT    ┌──────────────┐

│   STM32F4    │◄────────────►│   ESP32-AT   │◄─────────────►│  AWS IoT     │

│  Nucleo-F446RE│  AT Commands │  WROOM-32    │   Port 8883   │   Core       │

│              │              │              │               │              │

│  • FreeRTOS  │              │  • Wi-Fi     │               │  • MQTT      │

│  • DHT11     │              │  • TLS 1.2   │               │  • Rules     │

│  • DMA+IDLE  │              │  • X.509     │               │  • Shadows   │

└──────────────┘              └──────────────┘               └──────────────┘

## Architecture

The system implements a producer-consumer pattern using three FreeRTOS tasks communicating through message queues:

**Task 1 — Sensor Publisher (Priority 2):** Reads temperature and humidity from the DHT11 sensor every 10 seconds, formats the data as JSON, and enqueues it for transmission.

**Task 2 — Message Receiver (Priority 2):** Dequeues inbound MQTT messages received from AWS IoT Core and processes them (logging, command handling).

**Task 3 — AT Command Dispatcher (Priority 1):** Serializes all UART access to the ESP32. Dequeues outbound publish/subscribe requests, sends the corresponding AT commands, and polls for incoming MQTT data. This task runs at the lowest priority to ensure application tasks are never blocked by I/O.

DMA with idle-line detection on UART4 provides non-blocking reception of AT responses into a 10 KB ring buffer.

## Hardware

| Component | Part | Interface |
| :---- | :---- | :---- |
| MCU | STM32 Nucleo-F446RE (Cortex-M4, 160 MHz) | — |
| Wi-Fi Bridge | ESP32-WROOM-32 (ELEGOO) running ESP-AT firmware | UART4 (PA0 TX → GPIO16 RX, PA1 RX ← GPIO17 TX) |
| Temp/Humidity Sensor | DHT11 (single-wire, ±2°C / ±5% RH) | GPIO PA4 with DWT microsecond timing |
| Debug Console | ST-Link V2 Virtual COM Port | USART2 (PA2/PA3), 115200 baud |
| Reset Control | Nucleo NRST → ESP32 EN | GPIO |

### Wiring Diagram

STM32 Nucleo-F446RE          ESP32-WROOM-32

┌────────────────┐           ┌────────────────┐

│ 5V        ─────────────────── VIN            │

│ GND       ─────────────────── GND            │

│ NRST      ─────────────────── EN             │

│ A0 (PA0)  ─────────────────── RX2 (GPIO16)   │

│ A1 (PA1)  ─────────────────── TX2 (GPIO17)   │

└────────────────┘           └────────────────┘

STM32 Nucleo-F446RE          DHT11 Module

┌────────────────┐           ┌────────────────┐

│ 3V3       ─────────────────── VCC            │

│ GND       ─────────────────── GND            │

│ A2 (PA4)  ─────────────────── S (Signal)     │

└────────────────┘           └────────────────┘

## Software Stack

| Layer | Component | Details |
| :---- | :---- | :---- |
| RTOS | FreeRTOS 10.x | 3 tasks, 2 queues, heap\_4 allocator |
| Connectivity | ESP-AT Firmware | AT command interface over UART, TLS 1.2 mutual authentication |
| Protocol | MQTT 3.1.1 | QoS 0 publish, QoS 1 subscribe, LWT support |
| Security | X.509 Certificates | Device cert \+ private key \+ Amazon Root CA, embedded in ESP32 NVS partition |
| Sensor Driver | Custom DHT11 | Bit-banged single-wire protocol using DWT cycle counter for µs timing |
| HAL | STM32 HAL | UART, DMA, GPIO, I2C, TIM |
| Cloud | AWS IoT Core | MQTT broker, device registry, IoT policies |

## Data Flow

1. DHT11 sensor is read via single-wire GPIO protocol (20ms start pulse \+ 40-bit data frame)  
2. Sensor task formats JSON payload: `{"temp": 25, "humidity": 48}`  
3. Payload is enqueued to the MQTT TX queue (FreeRTOS `xQueueSend`)  
4. AT dispatcher task dequeues and sends `AT+MQTTPUBRAW` to ESP32 over UART4  
5. ESP32 encrypts with TLS 1.2 and publishes to AWS IoT Core on port 8883  
6. AWS IoT Core routes the message to subscribed clients  
7. Since the device subscribes to its own topic, inbound messages arrive as `+MQTTSUBRECV` unsolicited responses  
8. AT dispatcher parses the response, enqueues to MQTT RX queue  
9. Receiver task dequeues and logs the message

## Configuration

Copy `application_config.h` and fill in your credentials:

\#define WIFI\_SSID                    "YOUR\_WIFI\_SSID"

\#define WIFI\_PASSWORD                "YOUR\_WIFI\_PASSWORD"

\#define MQTT\_BROKER                  "your-endpoint.iot.us-east-1.amazonaws.com"

\#define MQTT\_PORT                    8883

\#define CLIENT\_ID                    "my\_iot\_device\_1"

\#define UTC\_OFFSET                   \-400

\#define SENSOR\_DATA\_TOPIC            "sensors/room1/temp\_humidity"

\#define MQTT\_PUBLISH\_TIME\_BETWEEN\_MS 10000

## AWS IoT Setup

1. Create an IoT Thing in the AWS IoT Console  
2. Generate and download device certificate, private key, and Amazon Root CA (AmazonRootCA1.pem)  
3. Create and attach an IoT Policy allowing `iot:Connect`, `iot:Publish`, `iot:Subscribe`, `iot:Receive`  
4. Attach the policy to the certificate, and the certificate to the Thing

### ESP32 Certificate Provisioning

Certificates are embedded into the ESP-AT firmware NVS partition before flashing:

## Build & Flash (STM32)

1. Open the project in STM32CubeIDE  
2. Build: Project → Build All  
3. Flash: Run → Debug (ST-Link SWD)  
4. Monitor: `screen /dev/cu.usbmodem* (Port)`

## Serial Output

\[INFO\] \[MAIN\_APP\] Initializing Wi-Fi module...

\[INFO\] \[MAIN\_APP\] Wi-Fi module initialized successfully.

\[INFO\] \[MAIN\_APP\] Joining Access Point: 'MyNetwork' ...

\[INFO\] \[MAIN\_APP\] Successfully joined Access Point: MyNetwork

\[INFO\] \[MAIN\_APP\] SNTP configured \!

\[INFO\] \[MAIN\_APP\] SNTP time retrieved: Wed, 22 Apr 2026 18:35:24

\[INFO\] \[MAIN\_APP\] Connecting to MQTT broker at xxxxxx.iot.us-east-1.amazonaws.com:8883...

\[INFO\] \[MAIN\_APP\] Successfully connected to MQTT broker

\[INFO\] \[MAIN\_APP\] Subscribing to topic: sensors/room1/temp\_humidity

\[INFO\] \[MAIN\_APP\] Successfully Subscribed to topic: sensors/room1/temp\_humidity

\[INFO\] \[UTIL\]     Temperature: 76 C

\[INFO\] \[UTIL\]     Humidity: 52%

\[INFO\] \[MAIN\_APP\] MQTT Publish successful: Payload='{"temp": 76, "humidity": 52}'

## Key Technical Decisions

**Custom DHT11 sensor driver:** The DHT11 uses a proprietary single-wire protocol rather than I2C. This project includes a custom bit-banged driver using the Cortex-M4 DWT cycle counter for microsecond-accurate GPIO timing, with checksum verification and timeout handling.

**Standalone component wiring:** All connections between the STM32, ESP32, and sensor are made with individual jumper wires on a breadboard. This required understanding every electrical connection, pin mapping, and signal direction.

**Integer temperature formatting:** The STM32 build uses newlib-nano (`--specs=nano.specs`) which does not include float printf support by default. Temperature values are cast to integer for JSON formatting. Float support can be enabled with the `-u _printf_float` linker flag.

**20-second UART timeout:** TLS handshake with AWS IoT Core takes 5–15 seconds on the ESP32. The default 7-second timeout was insufficient and was increased to 20 seconds.

## Tools

- STM32CubeIDE 1.x (build, flash, debug)  
- ESP-IDF 5.4 \+ ESP-AT (ESP32 firmware build)  
- AWS IoT Console (Thing, certificate, and policy management)  
- screen / minicom (serial monitoring on macOS)

## License

MIT License — see [LICENSE](http://LICENSE) for details.

## Acknowledgments

Inspired by concepts from embedded IoT coursework. System architecture, hardware integration, sensor driver, and security debugging are original work.  
