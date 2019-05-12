#include "protocol.h"
#include "read_string.h"
#include "handle_packet.h"

void task_read_serial(void *pv_parameters);

void setup() {
  while (!Serial) ;
  Serial.begin(9600);

  Protocol::setup_packet_handling();

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
      Protocol::handle_packet(pck);
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void loop() {
}
