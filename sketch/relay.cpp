#include "relay.h"

#include "protocol.h"
#include "codes.h"

bool states[8];

void Relay::setup() {
  for (int8_t i = 0; i < 8; i++) {
    states[i] = false;

    digitalWrite(i + 3, HIGH);
    pinMode(i + 3, OUTPUT);
  }
}

void Relay::set_state(int16_t request_id, const char *message) {
  int8_t relay_id = message[0] - '0';
  int8_t state = !(message[1] - '0');

  if (relay_id < 0 || relay_id > 7) {
    Protocol::send_failure_response(request_id, ERROR_INVALID_RELAY_ID);
    return;
  }

  digitalWrite(10 - relay_id, state);
  states[relay_id] = !state;

  Protocol::send_success_response(request_id);
};

void Relay::get_state(int16_t request_id, const char *message) {
  int8_t relay_id = message[0] - '0';

  if (relay_id < 0 || relay_id > 7) {
    Protocol::send_failure_response(request_id, ERROR_INVALID_RELAY_ID);
    return;
  }

  bool state = states[relay_id];

  Protocol::send_success_response(request_id, state);
};

void Relay::toggle_state(int16_t request_id, const char *message) {
  int8_t relay_id = message[0] - '0';

  if (relay_id < 0 || relay_id > 7) {
    Protocol::send_failure_response(request_id, ERROR_INVALID_RELAY_ID);
    return;
  }

  bool state = states[relay_id];
  digitalWrite(10 - relay_id, !!state);
  states[relay_id] = !state;

  Protocol::send_success_response(request_id);
};
