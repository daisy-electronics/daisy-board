#include "protocol.h"
#include "read_string.h"

void task_read_serial(void *pv_parameters);

void setup() {
  while (!Serial) ;
  Serial.begin(9600);

  SerialMutex::setup();

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
      // TODO: analyse packet
      {
        SerialMutex sm;
        Serial.println(pck.type);
        Serial.println(pck.request_id);
        Serial.println(pck.subject);
        Serial.println(pck.message);
      }
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void loop() {
}
