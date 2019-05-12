#pragma once

#include <Arduino.h>

namespace ReadSoilMoisture {
  void setup();
  void read(int16_t request_id, const char *message);
};
