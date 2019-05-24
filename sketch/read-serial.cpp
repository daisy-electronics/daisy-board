#include "read-serial.h"
#include <Arduino_FreeRTOS.h>
#include <Arduino.h>
#include "protocol.h"

namespace ReadSerial {
  namespace {
    void task_read_serial(void *pv_parameters);
  };

  void start() {
    xTaskCreate(
      task_read_serial,
      (const portCHAR *) "READ_SERIAL",
      128,
      nullptr,
      2,
      nullptr
    );
  }

  namespace {
    void task_read_serial(void *pv_parameters) {
      while (true) {
        if (Serial.available() > 0) {
          const char ch = Serial.read();
          Protocol::put(ch);
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
      }
    }
  };
};
