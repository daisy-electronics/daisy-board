#include "relay.h"

#include <EEPROM.h>
#include "protocol_internal.h"
#include "protocol_constants.h"

namespace {
  uint8_t relay_id2pin(uint8_t relay_id);
  uint8_t eeprom_read_relay_state(uint8_t relay_id);
  uint8_t eeprom_write_relay_state(uint8_t relay_id, uint8_t relay_state);
};

void Relay::setup() {
  for (int8_t relay_id = 0; relay_id < 8; relay_id++) {
    const bool relay_state = eeprom_read_relay_state(relay_id);
    pinMode(relay_id2pin(relay_id), OUTPUT);
    digitalWrite(relay_id2pin(relay_id), relay_state);
  }
}

void Relay::read_set_relay_data(uint8_t bit) {
  static enum {
    RelayIdReading,
    StateReading
  } state = RelayIdReading;

  static uint8_t i = 0;
  static uint8_t relay_id = 0;
  static uint8_t relay_state = 0;

  if (state == RelayIdReading) {
    bitWrite(relay_id, 2 - i, bit);
    i++;

    if (i == 3) {
      i = 0;
      state = StateReading;
    }
  } else if (state == StateReading) {
    bitWrite(relay_state, 0, !bit); // 0b00000001 is open

    if (relay_id < 0 || relay_id > 7) {
      uint8_t data[] = { RELAY_ERROR_INVALID_RELAY_ID << 7 };
      Protocol::send_failure_response(data, 1);
    } else {
      eeprom_write_relay_state(relay_id, relay_state);
      digitalWrite(relay_id2pin(relay_id), relay_state ? HIGH : LOW);
      Protocol::send_success_response();
    }

    // reinitialize static variables
    i = 0;
    state = RelayIdReading;
    relay_id = 0;
    relay_state = 0;

    Protocol::_state = Protocol::State::RedundantBitsSkipping;
  }
}

void Relay::read_get_relay_data(uint8_t bit) {
  static uint8_t i = 0;
  static uint8_t relay_id = 0;

  bitWrite(relay_id, 2 - i, bit);
  i++;

  if (i == 3) {
    if (relay_id < 0 || relay_id > 7) {
      uint8_t data[] = { RELAY_ERROR_INVALID_RELAY_ID << 7 };
      Protocol::send_failure_response(data, 1);
    } else {
      uint8_t data[] = { !eeprom_read_relay_state(relay_id) << 7 }; // { {0|1}0000000 }
      Protocol::send_success_response(data, 1);
    }

    // reinitialize static variables
    i = 0;
    relay_id = 0;

    Protocol::_state = Protocol::State::RedundantBitsSkipping;
  }
}

void Relay::read_toggle_relay_data(uint8_t bit) {
  static uint8_t i = 0;
  static uint8_t relay_id = 0;

  bitWrite(relay_id, 2 - i, bit);
  i++;

  if (i == 3) {
    if (relay_id < 0 || relay_id > 7) {
      uint8_t data[] = { RELAY_ERROR_INVALID_RELAY_ID << 7 };
      Protocol::send_failure_response(data, 1);
    } else {
      uint8_t relay_state = eeprom_read_relay_state(relay_id);
      eeprom_write_relay_state(relay_id, !relay_state);
      digitalWrite(relay_id2pin(relay_id), !relay_state ? HIGH : LOW);
      Protocol::send_success_response();
    }

    // reinitialize static variables
    i = 0;
    relay_id = 0;

    Protocol::_state = Protocol::State::RedundantBitsSkipping;
  }
}

namespace {
  inline uint8_t relay_id2pin(uint8_t relay_id) {
    return 10 - relay_id;
  }

  uint8_t eeprom_read_relay_state(uint8_t relay_id) {
    return EEPROM.read(RELAY_EEPROM_START + relay_id);
  }

  uint8_t eeprom_write_relay_state(uint8_t relay_id, uint8_t relay_state) {
    EEPROM.write(RELAY_EEPROM_START + relay_id, relay_state);
  }
};
