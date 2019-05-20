#pragma once

#include <Arduino.h>

namespace SoilMoisture {
  void setup();
  void read_get_soil_moisture_data(uint8_t bit);
};
