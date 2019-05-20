#pragma once

#include <Arduino.h>

namespace DS18B20 {
  void setup();
  void read_get_ds18b20_data(uint8_t bit);
};
