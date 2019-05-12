#include "read_soil_moisture.h"

#include "protocol.h"
#include "codes.h"

constexpr int16_t MOISTURE_LOWER_LIMIT = 448;
constexpr int16_t MOISTURE_UPPER_LIMIT = 815;
constexpr int16_t INTERVAL = MOISTURE_UPPER_LIMIT - MOISTURE_LOWER_LIMIT;

float from_raw(int16_t moisture);

void ReadSoilMoisture::setup() {
}

void ReadSoilMoisture::read(int16_t request_id, const char *message) {
  int8_t sensor_id = message[0] - '0';

  if (sensor_id < 0 || sensor_id > 1) {
    Protocol::send_failure_response(request_id, ERROR_INVALID_SOIL_MOISTURE_SENSOR_ID);
    return;
  }

  float moisture = from_raw(analogRead(sensor_id));
  Protocol::send_success_response(request_id, moisture);
};

inline float from_raw(int16_t moisture) {
  const float tmp = min(max(0, moisture - MOISTURE_LOWER_LIMIT), INTERVAL);
  return 100 - tmp / INTERVAL * 100;
}
