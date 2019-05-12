#include "set_relay.h"

#include "protocol.h"
#include "codes.h"

void SetRelay::setup() {
  for (int8_t i = 3; i < 11; i++) {
    digitalWrite(i, HIGH);
    pinMode(i, OUTPUT);
  }
}

void SetRelay::do_sync(uint32_t request_id, const char *message) {
  int8_t relay_id = message[0] - '0';
  int8_t state = !(message[1] - '0');

  if (relay_id < 0 || relay_id > 7) {
    Protocol::send_failure_response(request_id, ERROR_INVALID_RELAY_ID);
    return;
  }

  digitalWrite(10 - relay_id, state);

  Protocol::send_success_response(request_id);
};
