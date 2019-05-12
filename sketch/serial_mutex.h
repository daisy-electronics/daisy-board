#pragma once

#include <Arduino_FreeRTOS.h>
#include <semphr.h>

class SerialMutex {
  static SemaphoreHandle_t mutex_handle;
public:
  SerialMutex();
  ~SerialMutex();
};
