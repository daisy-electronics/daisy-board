#include <Arduino_FreeRTOS.h>
#include "protocol.h"
#include "relay.h"
#include "soil_moisture.h"
#include "dht_sensor.h"
#include "ds18b20.h"

void task_read_serial(void *pv_parameters);

void setup() {
  while (!Serial);
  Serial.begin(9600);

  Relay::setup();
  SoilMoisture::setup();
  DHT::setup();
  DS18B20::setup();

  xTaskCreate(
    task_read_serial,
    (const portCHAR *) "READ_SERIAL",
    128,
    nullptr,
    2,
    nullptr
  );
}

void task_read_serial(void *pv_parameters) {
  while (true) {
    if (Serial.available() > 0) {
      const char ch = Serial.read();
      Protocol::put(ch);
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void loop() {
}
