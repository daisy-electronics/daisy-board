#include "ds18b20.h"

#include <Arduino_FreeRTOS.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "protocol_internal.h"
#include "protocol_constants.h"
#include "bitmemset.h"

#define BUS0 49

namespace {
  void task_broadcast(void *pv_parameters);
  void read_one_sensor(uint8_t sensor_id, float *temperature);

  OneWire oneWire(BUS0);
  DallasTemperature sensors(&oneWire);

  float data_temperature[] = { 0 };
};


void DS18B20::setup() {
  sensors.begin();

  //xTaskCreate(
  //  task_broadcast,
  //  (const portCHAR *) "BROADCAST_DS18B20",
  //  128,
  //  nullptr,
  //  1,
  //  nullptr
  //);
}

void DS18B20::read_get_ds18b20_data(uint8_t bit) {
  static uint8_t i = 0;
  static uint8_t sensor_id = 0;

  bitWrite(sensor_id, 2 - i, bit);
  i++;

  if (i == 3) {
    if (sensor_id < 0 || sensor_id > 0) {
      uint8_t data[] = { DS18B20_ERROR_INVALID_SENSOR_ID << 7 };
      Protocol::send_failure_response(data, 1);
    } else {
      read_one_sensor(sensor_id, &data_temperature[sensor_id]);

      union {
        uint16_t temperature;
        uint8_t temperature_split[2];
      };
      temperature = (data_temperature[sensor_id] + 55) * 2;

      uint8_t data[] = { 0, 0 };
      // crazy stuff here because ATmega2560 is little-endian
      bitmemset_v(data, 0, temperature_split[1], 8 - 1, 1);
      bitmemset_v(data, 1, temperature_split[0], 8 - 8, 8);

      Protocol::send_success_response(data, 9);
    }

    // reinitialize static variables
    i = 0;
    sensor_id = 0;

    Protocol::_state = Protocol::State::RedundantBitsSkipping;
  }
}

namespace {
  void task_broadcast(void *vp_parameters) {
    while (true) {
      sensors.requestTemperatures();

      for (uint8_t sensor_id = 0; sensor_id < 1; sensor_id++) {
        data_temperature[sensor_id] = sensors.getTempCByIndex(sensor_id);

        union {
          uint16_t temperature;
          uint8_t temperature_split[2];
        };
        temperature = (data_temperature[sensor_id] + 55) * 2;

        uint8_t data[] = { 0, 0 };
        bitmemset_v(data, 0, sensor_id, 8 - 3, 3);

        // crazy stuff because ATmega2560 is little-endian
        bitmemset_v(data, 3, temperature_split[1], 8 - 1, 1);
        bitmemset_v(data, 3 + 1, temperature_split[0], 8 - 8, 8);

        Protocol::emit_event(OUTBOUND_EVENT_DS18B20, data, 3 + 9);
      }

      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
  }

  void read_one_sensor(uint8_t sensor_id, float *temperature) {
    sensors.requestTemperatures();

    *temperature = sensors.getTempCByIndex(sensor_id);
  }
};
