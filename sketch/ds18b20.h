#pragma once

#include <Arduino.h>

namespace DS18B20 {
  void setup();
  void read(int16_t request_id, const char *message);
};
