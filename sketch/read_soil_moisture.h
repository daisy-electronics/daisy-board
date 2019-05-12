#pragma once

#include <Arduino.h>

namespace ReadSoilMoisture {
  void setup();
  void do_sync(uint32_t request_id, const char *message);
};
