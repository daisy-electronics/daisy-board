#pragma once

#include <Arduino.h>

namespace SoilMoisture {
  void setup();
  void read(int16_t request_id, const char *message);
  void read_and_emit(int8_t sensor_id);
};
