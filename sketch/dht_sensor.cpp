#include "dht_sensor.h"

#include <DHT.h>
#include "protocol.h"
#include "codes.h"

#define SENSOR0 51 // DHT22
#define SENSOR1 53 // DHT22

void readSensor(int16_t request_id, int8_t sensor_id);

DHT sensors[] = {
  DHT(SENSOR0, DHT22),
  DHT(SENSOR1, DHT22)
};

void DHTSensor::setup() {
  for (int8_t i = 0; i < 2; i++) {
    sensors[i].begin();
  }
}

void DHTSensor::read(int16_t request_id, const char *message) {
  int8_t sensor_id = message[0] - '0';

  if (sensor_id < 0 || sensor_id > 1) {
    Protocol::send_failure_response(request_id, ERROR_INVALID_DHT_SENSOR_ID);
    return;
  }

  readSensor(request_id, sensor_id);
}

void readSensor(int16_t request_id, int8_t sensor_id) {
  const DHT &sensor = sensors[sensor_id];
  const float humidity = sensor.readHumidity();
  const float temperature = sensor.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Protocol::send_failure_response(request_id, ERROR_READ_DHT_SENSOR);
    return;
  }

  static char str_humidity[5];
  static char str_temperature[5];
  dtostrf(humidity, 4, 1, str_humidity);
  dtostrf(temperature, 4, 1, str_temperature);
  Protocol::send_success_response(request_id, str_humidity, false);
  Serial.print(" ");
  Serial.println(str_temperature);
}
