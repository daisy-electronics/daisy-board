#include "dht_sensor.h"

#include "dht.h"
#include "protocol.h"
#include "codes.h"

#define SENSOR0 51 // DHT22
#define SENSOR1 53 // DHT22

dht sensor;

float data_humidity[] = { 0, 0 };
float data_temperature[] = { 0, 0 };

int8_t sensor_id2pin(int8_t);

void DHTSensor::setup() {
}

void DHTSensor::read(int16_t request_id, const char *message) {
  int8_t sensor_id = message[0] - '0';

  if (sensor_id < 0 || sensor_id > 1) {
    Protocol::send_failure_response(request_id, ERROR_INVALID_DHT_SENSOR_ID);
    return;
  }

  static char str_humidity[5];
  static char str_temperature[5];
  dtostrf(data_humidity[sensor_id], 4, 1, str_humidity);
  dtostrf(data_temperature[sensor_id], 4, 1, str_temperature);
  Protocol::send_success_response(request_id, str_humidity, str_temperature);
}

void DHTSensor::read_and_emit(int8_t sensor_id) {
  int res = sensor.read22(sensor_id2pin(sensor_id));
  if (res != DHTLIB_OK) {
    Protocol::emit_event(EVENT_ERROR, ERROR_READ_DHT_SENSOR);
    return;
  }

  data_humidity[sensor_id] = sensor.humidity;
  data_temperature[sensor_id] = sensor.temperature;

  static char str_humidity[5];
  static char str_temperature[5];
  dtostrf(sensor.humidity, 4, 1, str_humidity);
  dtostrf(sensor.temperature, 4, 1, str_temperature);
  Protocol::emit_event(EVENT_DHT, sensor_id, str_humidity, str_temperature);
}

int8_t sensor_id2pin(int8_t sensor_id) {
  if (sensor_id == 0) {
    return SENSOR0;
  } else if (sensor_id == 1) {
    return SENSOR1;
  } else {
    return -1;
  }
}
