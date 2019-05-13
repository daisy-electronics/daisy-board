#include "handle_packet.h"

#include "dht_sensor.h"
#include "relay.h"
#include "read_soil_moisture.h"
#include "codes.h"

void Protocol::setup_packet_handling() {
  DHTSensor::setup();
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
    // read dht
    if (strcmp(pck.subject, REQUEST_READ_DHT) == 0) {
      DHTSensor::read(pck.request_id, pck.message);

    // set relay
    } else if (strcmp(pck.subject, REQUEST_SET_RELAY) == 0) {
      Relay::set_state(pck.request_id, pck.message);

    // get relay
    } else if (strcmp(pck.subject, REQUEST_GET_RELAY) == 0) {
      Relay::get_state(pck.request_id, pck.message);


    // toggle relay
    } else if (strcmp(pck.subject, REQUEST_TOGGLE_RELAY) == 0) {
      Relay::toggle_state(pck.request_id, pck.message);

    // read soil moisture
    } else if (strcmp(pck.subject, REQUEST_READ_SOIL_MOISTURE) == 0) {
      ReadSoilMoisture::read(pck.request_id, pck.message);
    }
  }
}
