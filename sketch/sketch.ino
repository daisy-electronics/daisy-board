#include "protocol.h"
#include "read_string.h"
#include "handle_packet.h"

void read_serial();

void setup() {
  while (!Serial) ;
  Serial.begin(9600);

  Protocol::setup_packet_handling();
}

void read_serial() {
  static char received_string[Protocol::MESSAGE_MAX_LENGTH] = { '\0' };
  static Protocol::Packet pck;

  read_string(received_string);
  if (received_string[0] != '\0') {
    Protocol::Packet::parse(received_string, pck);
    Protocol::handle_packet(pck);
  }
}

void loop() {
  read_serial();
  delay(100);
}
