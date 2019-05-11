#include "serial_mutex.h"

static SemaphoreHandle_t SerialMutex::semaphore = nullptr;

static void SerialMutex::setup() {
  semaphore = xSemaphoreCreateMutex();
}

SerialMutex::SerialMutex() {
  while (xSemaphoreTake(semaphore, (TickType_t) 5) != pdTRUE) {
    vTaskDelay(1);
  }
}

SerialMutex::~SerialMutex() {
  xSemaphoreGive(semaphore);
}
