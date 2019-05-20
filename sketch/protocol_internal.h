#pragma once

#include <Arduino.h>

namespace Protocol {
  enum State {
    Idle,
    PacketTypeReading,
    SubjectReading,
    DataReading,
    RedundantBitsSkipping
  };

  extern State _state;

  void emit_event(uint8_t subject, uint8_t data[] = nullptr, uint8_t data_len = 0);
  void send_success_response(uint8_t data[] = nullptr, uint8_t data_len = 0);
  void send_failure_response(uint8_t data[] = nullptr, uint8_t data_len = 0);
};
