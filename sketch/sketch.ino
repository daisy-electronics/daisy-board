#include "protocol.h"
#include "read_string.h"
#include "handle_packet.h"
#include "soil_moisture.h"
#include "dht_sensor.h"

void task_read_serial(void *pv_parameters);
void task_emit_soil_moisture(void *pv_parameters);
void task_emit_dht(void *pv_parameters);

void setup() {
  while (!Serial) ;
  Serial.begin(9600);

  Protocol::setup_packet_handling();

  xTaskCreate(
    task_read_serial,
    (const portCHAR *) "ReadSerial",
    128,
    nullptr,
    2,
    nullptr
  );

  xTaskCreate(
    task_emit_soil_moisture,
    (const portCHAR *) "EmitSoilMoisture",
    128,
    nullptr,
    1,
    nullptr
  );

  xTaskCreate(
    task_emit_dht,
    (const portCHAR *) "EmitDHT",
    128,
    nullptr,
    1,
    nullptr
  );
}

void task_read_serial(void *pv_parameters) {
  static char received_string[Protocol::MESSAGE_MAX_LENGTH] = { '\0' };
  static Protocol::Packet pck;

  while (true) {
    read_string(received_string);
    if (received_string[0] != '\0') {
      Protocol::Packet::parse(received_string, pck);
      Protocol::handle_packet(pck);
    }

    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void task_emit_soil_moisture(void *pv_parameters) {
  while (true) {
    SoilMoisture::read_and_emit(0);
    SoilMoisture::read_and_emit(1);

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void task_emit_dht(void *pv_parameters) {
  while (true) {
    DHTSensor::read_and_emit(0);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    DHTSensor::read_and_emit(1);

    vTaskDelay(3000 / portTICK_PERIOD_MS);
  }
}

void loop() {
}
