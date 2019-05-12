#include "handle_packet.h"

#include "set_relay.h"
#include "read_soil_moisture.h"
#include "codes.h"

void Protocol::setup_packet_handling() {
  SetRelay::setup();
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
      SetRelay::do_sync(pck.request_id, pck.message);

    // read soil moisture
    } else if (strcmp(pck.subject, REQUEST_READ_SOIL_MOISTURE) == 0) {
      ReadSoilMoisture::do_sync(pck.request_id, pck.message);
    }
  }
}
