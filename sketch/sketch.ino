#include "protocol.h"
#include "read_string.h"
#include "set_relay.h"
#include "read_soil_moisture.h"

void task_read_serial(void *pv_parameters);

void setup() {
  while (!Serial) ;
  Serial.begin(9600);

  SerialMutex::setup();
  SetRelay::setup();
  ReadSoilMoisture::setup();

  xTaskCreate(
    task_read_serial,
    (const portCHAR *) "read_serial",
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

      if (pck.type == Protocol::EVENT) {
      } else if (pck.type == Protocol::REQUEST) {
        // set relay
        if (strcmp(pck.subject, SetRelay::request_subject) == 0) {
          SetRelay::do_sync(pck.request_id, pck.message);

        // read soil moisture
        } else if (strcmp(pck.subject, ReadSoilMoisture::request_subject) == 0) {
          ReadSoilMoisture::do_sync(pck.request_id, pck.message);
        }
      }
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void loop() {
}
