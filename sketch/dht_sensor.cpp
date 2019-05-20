#include "dht_sensor.h"

#include <Arduino_FreeRTOS.h>
#include "dht.h"
#include "protocol_internal.h"
#include "protocol_constants.h"
#include "bitmemset.h"

#define SENSOR0 51 // DHT22
#define SENSOR1 53 // DHT22

namespace {
  uint8_t sensor_id2pin(uint8_t sensor_id);
  void task_broadcast(void *pv_parameters);
  bool read(uint8_t sensor_id, float *humidity, float *temperature);
  void read_and_emit(uint8_t sensor_id);

  dht sensor;

  float data_humidity[] = { 0, 0 };
  float data_temperature[] = { 0, 0 };
};

void DHT::setup() {
  //xTaskCreate(
  //  task_broadcast,
  //  (const portCHAR *) "BROADCAST_DHT",
  //  128,
  //  nullptr,
  //  1,
  //  nullptr
  //);
}

void DHT::read_get_dht_data(uint8_t bit) {
  static uint8_t i = 0;
  static uint8_t sensor_id = 0;

  bitWrite(sensor_id, 2 - i, bit);
  i++;

  if (i == 3) {
    if (sensor_id < 0 || sensor_id > 1) {
      uint8_t data[] = { DHT_ERROR_INVALID_SENSOR_ID << 7 };
      Protocol::send_failure_response(data, 1);
    } else {
      if (!read(sensor_id, &data_humidity[sensor_id], &data_temperature[sensor_id])) {
        // TODO: emit error reading dht sensor
      }
      const uint8_t humidity = data_humidity[sensor_id];
      const uint8_t temperature = (data_temperature[sensor_id] + 40) * 2;

      uint8_t data[] = { 0, 0 };
      bitmemset_v(data, 0, humidity, 8 - 7, 7);
      bitmemset_v(data, 7, temperature, 8 - 8, 8);

      Protocol::send_success_response(data, 7 + 8);
    }

    // reinitialize static variables
    i = 0;
    sensor_id = 0;

    Protocol::_state = Protocol::State::RedundantBitsSkipping;
  }
}

namespace {
  void task_broadcast(void *pv_parameters) {
    while (true) {
      read_and_emit(0);
      read_and_emit(1);

      vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
  }

  void read_and_emit(uint8_t sensor_id) {
    if (!read(sensor_id, &data_humidity[sensor_id], &data_temperature[sensor_id])) {
      // TODO: emit error reading dht sensor
      return;
    }

    const uint8_t humidity = sensor.humidity;
    const uint8_t temperature = (sensor.temperature + 40) * 2;

    uint8_t data[] = { 0, 0, 0 };
    bitmemset_v(data, 0, sensor_id, 8 - 3, 3);
    bitmemset_v(data, 3, humidity, 8 - 7, 7);
    bitmemset_v(data, 3 + 7, temperature, 8 - 8, 8);

    Protocol::emit_event(OUTBOUND_EVENT_DHT, data, 3 + 7 + 8);
  }

  bool read(uint8_t sensor_id, float *humidity, float *temperature) {
    int res = sensor.read22(sensor_id2pin(sensor_id));
    if (res != DHTLIB_OK) {
      return false;
    }

    *humidity = sensor.humidity;
    *temperature = sensor.temperature;

    return true;
  }

  inline uint8_t sensor_id2pin(uint8_t sensor_id) {
    if (sensor_id == 0) {
      return SENSOR0;
    } else if (sensor_id == 1) {
      return SENSOR1;
    }
  }
};
