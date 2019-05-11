#pragma once

#include <Arduino_FreeRTOS.h>
#include <semphr.h>

class SerialMutex {
  static SemaphoreHandle_t semaphore;
public:
  static void setup();
  SerialMutex();
  ~SerialMutex();
};
