#pragma once

#include <Arduino.h>

namespace Protocol {
  constexpr uint8_t EVENT = 0;
  constexpr uint8_t REQUEST = 1;
  constexpr uint8_t SUCCESS_RESPONSE = 2;
  constexpr uint8_t FAILURE_RESPONSE = 3;
  constexpr uint8_t INVALID = 9;

  constexpr uint16_t PACKET_MAX_LENGTH = 48;
  constexpr uint16_t REQUEST_ID_MAX_LENGTH = 8;
  constexpr uint16_t SUBJECT_MAX_LENGTH = 16;
  constexpr uint16_t MESSAGE_MAX_LENGTH = PACKET_MAX_LENGTH - REQUEST_ID_MAX_LENGTH - SUBJECT_MAX_LENGTH - 1 - 1;

  struct Packet {
    uint8_t type = INVALID;
    uint32_t request_id = 0;
    char subject[SUBJECT_MAX_LENGTH];
    char message[MESSAGE_MAX_LENGTH];

    static void parse(const char *str, Packet &pck);
  };
};
