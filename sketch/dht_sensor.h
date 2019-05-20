#pragma once

#include <Arduino.h>

namespace DHT {
  void setup();
  void read_get_dht_data(uint8_t bit);
};
