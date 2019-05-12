#pragma once

#include <Arduino.h>

namespace Relay {
  void setup();
  void set_state(int16_t request_id, const char *message);
  void get_state(int16_t request_id, const char *message);
  void toggle_state(int16_t request_id, const char *message);
};
