#include "handle_packet.h"

#include "relay.h"
#include "read_soil_moisture.h"
#include "codes.h"

void Protocol::setup_packet_handling() {
  Relay::setup();
  ReadSoilMoisture::setup();
}

void Protocol::handle_packet(const Protocol::Packet &pck) {
  //
  // EVENT
  //
  if (pck.type == EVENT) {

  //
  // REQUEST
  //
  } else if (pck.type == REQUEST) {

    // set relay
    if (strcmp(pck.subject, REQUEST_SET_RELAY) == 0) {
      Relay::set_state(pck.request_id, pck.message);

    // get relay
    } else if (strcmp(pck.subject, REQUEST_GET_RELAY) == 0) {
      Relay::get_state(pck.request_id, pck.message);

    // read soil moisture
    } else if (strcmp(pck.subject, REQUEST_READ_SOIL_MOISTURE) == 0) {
      ReadSoilMoisture::read(pck.request_id, pck.message);
    }
  }
}
