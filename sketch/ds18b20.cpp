#include "ds18b20.h"

#include <Arduino_FreeRTOS.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "protocol.h"
#include "codes.h"

#define BUS0 49

static void task_broadcast(void *pv_parameters);

static OneWire oneWire(BUS0);
static DallasTemperature sensors(&oneWire);

static float data_temperature[1] = { 0 };

void DS18B20::setup() {
  sensors.begin();

  xTaskCreate(
    task_broadcast,
    (const portCHAR *) "BROADCAST_DS18B20",
    128,
    nullptr,
    1,
    nullptr
  );
}

static void task_broadcast(void *vp_parameters) {
  while (true) {
    sensors.requestTemperatures();

    for (int8_t sensor_id = 0; sensor_id < 1; sensor_id++) {
      float temperature = sensors.getTempCByIndex(sensor_id);
      data_temperature[sensor_id] = temperature;

      static char str_temperature[6];
      dtostrf(temperature, 4, 2, str_temperature);
      Protocol::emit_event(EVENT_DS18B20, sensor_id, str_temperature);
    }

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void DS18B20::read(int16_t request_id, const char *message) {
  int8_t sensor_id = message[0] - '0';

  if (sensor_id < 0 || sensor_id > 0) {
    Protocol::send_failure_response(request_id, ERROR_INVALID_DS18B20_SENSOR_ID);
    return;
  }

  static char str_temperature[6];
  dtostrf(data_temperature[sensor_id], 4, 2, str_temperature);
  Protocol::send_success_response(request_id, str_temperature);
}
