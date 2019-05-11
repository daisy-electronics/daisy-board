#include "packet.h"

bool isnum(char ch);

static void Protocol::Packet::parse(const char *str, Packet &pck) {
  pck.type = INVALID;
  pck.request_id = 0;
  pck.subject[0] = '\0';
  pck.message[0] = '\0';

  if (str[0] == '\0') {
    pck.type = INVALID;
    return;

  //
  // EVENT
  //
  } else if (str[0] == '0' + EVENT) {
    pck.type = EVENT;

    // Read subject

    uint16_t current_str_i = 0;
    uint16_t i = 1;
    char ch;
    while ((ch = str[i++]) != '|' && ch != '\0' && current_str_i < SUBJECT_MAX_LENGTH - 1) {
      pck.subject[current_str_i++] = ch;
    }
    pck.subject[current_str_i] = '\0';

    // If end, then no message

    if (ch == '\0') {
      pck.type = INVALID;
      return;
    }

    // Read message

    current_str_i = 0;
    while ((ch = str[i++]) != '\0' && current_str_i < MESSAGE_MAX_LENGTH - 1) {
      pck.message[current_str_i++] = ch;
    }
    pck.message[current_str_i] = '\0';

  //
  // REQUEST
  //
  } else if (str[0] == '0' + REQUEST) {
    pck.type = REQUEST;

    // Read request_id

    uint16_t i = 1;
    char ch;
    while (isnum(ch = str[i++]) && i < REQUEST_ID_MAX_LENGTH - 1) ;
    if (ch != '|') {
      pck.type = INVALID;
      return;
    }

    for (uint8_t request_id_i = 0; request_id_i < i - 2; request_id_i++) {
      uint32_t k = 1;
      for (uint8_t j = 0; j < request_id_i; j++) k *= 10;

      pck.request_id += k * (str[i - 2 - request_id_i] - '0');
    }

    // Read subject

    uint16_t current_str_i = 0;
    while ((ch = str[i++]) != '|' && ch != '\0' && current_str_i < SUBJECT_MAX_LENGTH - 1) {
      pck.subject[current_str_i++] = ch;
    }
    pck.subject[current_str_i] = '\0';

    // If end, then no message

    if (ch == '\0') {
      return;
    }

    // Read message

    current_str_i = 0;
    while ((ch = str[i++]) != '|' && ch != '\0' && current_str_i < MESSAGE_MAX_LENGTH - 1) {
      pck.message[current_str_i++] = ch;
    }
    pck.message[current_str_i] = '\0';

  //
  // SUCCESS_RESPONSE
  //
  } else if (str[0] == '0' + SUCCESS_RESPONSE) {
    pck.type = SUCCESS_RESPONSE;

    // Read request_id

    uint16_t i = 1;
    char ch;
    while (isnum(ch = str[i++]) && i < REQUEST_ID_MAX_LENGTH - 1) ;
    if (ch != '|' && ch != '\0') {
      pck.type = INVALID;
      return;
    }

    for (uint8_t request_id_i = 0; request_id_i < i - 2; request_id_i++) {
      uint32_t k = 1;
      for (uint8_t j = 0; j < request_id_i; j++) k *= 10;

      pck.request_id += k * (str[i - 2 - request_id_i] - '0');
    }

    // If end, then no message

    if (ch == '\0') {
      return;
    }

    // Read message

    uint16_t current_str_i = 0;
    while ((ch = str[i++]) != '|' && ch != '\0' && current_str_i < MESSAGE_MAX_LENGTH - 1) {
      pck.message[current_str_i++] = ch;
    }
    pck.message[current_str_i] = '\0';

  //
  // FAILURE_RESPONSE
  //
  } else if (str[0] == '0' + FAILURE_RESPONSE) {
    pck.type = FAILURE_RESPONSE;

    // Read request_id

    uint16_t i = 1;
    char ch;
    while (isnum(ch = str[i++]) && i < REQUEST_ID_MAX_LENGTH - 1) ;
    if (ch != '|' && ch != '\0') {
      pck.type = INVALID;
      return;
    }

    for (uint8_t request_id_i = 0; request_id_i < i - 2; request_id_i++) {
      uint32_t k = 1;
      for (uint8_t j = 0; j < request_id_i; j++) k *= 10;

      pck.request_id += k * (str[i - 2 - request_id_i] - '0');
    }

    // If end, then no message

    if (ch == '\0') {
      return;
    }

    // Read message

    uint16_t current_str_i = 0;
    while ((ch = str[i++]) != '|' && ch != '\0' && current_str_i < MESSAGE_MAX_LENGTH - 1) {
      pck.message[current_str_i++] = ch;
    }
    pck.message[current_str_i] = '\0';
  }
}

inline bool isnum(char ch) {
  return ch >= '0' && ch <= '9';
}
