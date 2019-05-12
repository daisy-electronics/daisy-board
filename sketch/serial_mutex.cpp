#include "serial_mutex.h"

static SemaphoreHandle_t SerialMutex::mutex_handle = xSemaphoreCreateMutex();

SerialMutex::SerialMutex() {
  while (xSemaphoreTake(mutex_handle, (TickType_t) 5) != pdTRUE) {
    vTaskDelay(1);
  }
}

SerialMutex::~SerialMutex() {
  xSemaphoreGive(mutex_handle);
}
