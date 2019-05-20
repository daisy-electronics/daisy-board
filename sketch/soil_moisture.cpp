#include "soil_moisture.h"

#include <Arduino_FreeRTOS.h>
#include "protocol_internal.h"
#include "protocol_constants.h"
#include "bitmemset.h"

#define  MOISTURE_LOWER_LIMIT 448
#define  MOISTURE_UPPER_LIMIT 815
constexpr int16_t INTERVAL = MOISTURE_UPPER_LIMIT - MOISTURE_LOWER_LIMIT;

namespace {
  uint8_t from_raw(uint16_t moisture);
  void task_broadcast(void *pv_parameters);
  void read_and_emit(uint8_t sensor_id);
};

void SoilMoisture::setup() {
  //xTaskCreate(
  //  task_broadcast,
  //  (const portCHAR *) "BROADCAST_SOIL_MOISTURE",
  //  128,
  //  nullptr,
  //  1,
  //  nullptr
  //);
}

void SoilMoisture::read_get_soil_moisture_data(uint8_t bit) {
  static uint8_t i = 0;
  static uint8_t sensor_id = 0;

  bitWrite(sensor_id, 3 - i, bit);
  i++;

  if (i == 4) {
    if (sensor_id < 0 || sensor_id > 1) {
      uint8_t data[] = { SOIL_MOISTURE_ERROR_INVALID_SENSOR_ID << 7 };
      Protocol::send_failure_response(data, 1);
    } else {
      uint8_t data[] = { from_raw(analogRead(sensor_id)) << 1 };
      Protocol::send_success_response(data, 7);
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

      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
  }

  void read_and_emit(uint8_t sensor_id) {
    uint8_t moisture = from_raw(analogRead(sensor_id));

    uint8_t data[2] = { 0, 0 };
    bitmemset_v(data, 0, sensor_id, 8 - 4, 4);
    bitmemset_v(data, 4, moisture, 8 - 7, 7);

    Protocol::emit_event(OUTBOUND_EVENT_SOIL_MOISTURE, data, 4 + 7);
  }

  inline uint8_t from_raw(uint16_t moisture) {
    const float tmp = min(max(0, moisture - MOISTURE_LOWER_LIMIT), INTERVAL);
    return 100 - tmp / INTERVAL * 100;
  }
};
