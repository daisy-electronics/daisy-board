#pragma once

constexpr uint8_t EVENT = 0b00;
constexpr uint8_t REQUEST = 0b01;
constexpr uint8_t SUCCESS_RESPONSE = 0b10;
constexpr uint8_t FAILURE_RESPONSE = 0b11;

#define OUTBOUND_EVENT_SOIL_MOISTURE 0b0000
#define OUTBOUND_EVENT_DHT 0b0001
#define OUTBOUND_EVENT_DS18B20 0b0010

#define INBOUND_REQUEST_SET_RELAY 0b0000
#define INBOUND_REQUEST_GET_RELAY 0b0001
#define INBOUND_REQUEST_TOGGLE_RELAY 0b0010
#define INBOUND_REQUEST_GET_SOIL_MOISTURE 0b0011
#define INBOUND_REQUEST_GET_DHT 0b0100
#define INBOUND_REQUEST_GET_DS18B20 0b0101

#define RELAY_EEPROM_START 0 // end: +8 bytes
#define RELAY_ERROR_INVALID_RELAY_ID 0

#define SOIL_MOISTURE_ERROR_INVALID_SENSOR_ID 0

#define DHT_ERROR_INVALID_SENSOR_ID 0

#define DS18B20_ERROR_INVALID_SENSOR_ID 0