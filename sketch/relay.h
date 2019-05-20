#pragma once

#include <Arduino.h>

namespace Relay {
  void setup();
  void read_set_relay_data(uint8_t bit);
  void read_get_relay_data(uint8_t bit);
  void read_toggle_relay_data(uint8_t bit);
};
