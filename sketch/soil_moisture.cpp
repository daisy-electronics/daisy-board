#include "soil_moisture.h"

#include <Arduino_FreeRTOS.h>
#include "protocol.h"
#include "codes.h"

constexpr int16_t MOISTURE_LOWER_LIMIT = 448;
constexpr int16_t MOISTURE_UPPER_LIMIT = 815;
constexpr int16_t INTERVAL = MOISTURE_UPPER_LIMIT - MOISTURE_LOWER_LIMIT;

static float from_raw(int16_t moisture);
static void task_broadcast(void *pv_parameters);
static void read_and_emit(int8_t sensor_id);

void SoilMoisture::setup() {
  xTaskCreate(
    task_broadcast,
    (const portCHAR *) "BROADCAST_SOIL_MOISTURE",
    128,
    nullptr,
    1,
    nullptr
  );
}

static void task_broadcast(void *pv_parameters) {
  while (true) {
    read_and_emit(0);
    read_and_emit(1);

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}


void SoilMoisture::read(int16_t request_id, const char *message) {
  int8_t sensor_id = message[0] - '0';

  if (sensor_id < 0 || sensor_id > 1) {
    Protocol::send_failure_response(request_id, ERROR_INVALID_SOIL_MOISTURE_SENSOR_ID);
    return;
  }

  float moisture = from_raw(analogRead(sensor_id));
  Protocol::send_success_response(request_id, moisture);
}

static void read_and_emit(int8_t sensor_id) {
  float moisture = from_raw(analogRead(sensor_id));
  static char str_moisture[5];
  dtostrf(moisture, 4, 1, str_moisture);

  Protocol::emit_event(EVENT_SOIL_MOISTURE, sensor_id, str_moisture);
}

static inline float from_raw(int16_t moisture) {
  const float tmp = min(max(0, moisture - MOISTURE_LOWER_LIMIT), INTERVAL);
  return 100 - tmp / INTERVAL * 100;
}
