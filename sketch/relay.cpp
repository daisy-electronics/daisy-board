#include "relay.h"
#include <EEPROM.h>
#include "protocol.h"
#include "codes.h"

static int8_t relay_id2pin(int8_t relay_id);

void Relay::setup() {
  for (int8_t relay_id = 0; relay_id < 8; relay_id++) {
    const bool state = !!EEPROM.read(EEPROM_RELAY_START + relay_id);
    pinMode(relay_id2pin(relay_id), OUTPUT);
    digitalWrite(relay_id2pin(relay_id), state);
  }
}

void Relay::set_state(int16_t request_id, const char *message) {
  int8_t relay_id = message[0] - '0';
  bool state = !(message[1] - '0');

  if (relay_id < 0 || relay_id > 7) {
    Protocol::send_failure_response(request_id, ERROR_INVALID_RELAY_ID);
    return;
  }

  EEPROM.write(EEPROM_RELAY_START + relay_id, state);
  digitalWrite(relay_id2pin(relay_id), state);

  Protocol::send_success_response(request_id);
}

void Relay::get_state(int16_t request_id, const char *message) {
  int8_t relay_id = message[0] - '0';

  if (relay_id < 0 || relay_id > 7) {
    Protocol::send_failure_response(request_id, ERROR_INVALID_RELAY_ID);
    return;
  }

  bool state = !EEPROM.read(EEPROM_RELAY_START + relay_id);

  Protocol::send_success_response(request_id, state);
}

void Relay::toggle_state(int16_t request_id, const char *message) {
  int8_t relay_id = message[0] - '0';

  if (relay_id < 0 || relay_id > 7) {
    Protocol::send_failure_response(request_id, ERROR_INVALID_RELAY_ID);
    return;
  }

  bool state = EEPROM.read(EEPROM_RELAY_START + relay_id);
  EEPROM.write(EEPROM_RELAY_START + relay_id, !state);
  digitalWrite(relay_id2pin(relay_id), !state);

  Protocol::send_success_response(request_id);
}

static inline int8_t relay_id2pin(int8_t relay_id) {
  return 10 - relay_id;
}
