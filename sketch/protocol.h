#pragma once

#include <Arduino.h>
#include "packet.h"

namespace Protocol {
  template <typename Subject>
  void emit_event(Subject subject) {
    Serial.print(EVENT);
    Serial.println(subject);
  }

  template <typename Subject, typename Message>
  void emit_event(Subject subject, Message message) {
    Serial.print(EVENT);
    Serial.print(subject);
    Serial.print("|");
    Serial.println(message);
  }

  template <typename Subject>
  void send_request(int16_t request_id, Subject subject) {
    Serial.print(REQUEST);
    Serial.print(request_id);
    Serial.print("|");
    Serial.println(subject);
  }

  template <typename Subject, typename Message>
  void send_request(int16_t request_id, Subject subject, Message message) {
    Serial.print(REQUEST);
    Serial.print(request_id);
    Serial.print("|");
    Serial.print(subject);
    Serial.print("|");
    Serial.println(message);
  }

  void send_success_response(int16_t request_id);

  template <typename Message>
  void send_success_response(int16_t request_id, Message message, bool finish = true) {
    Serial.print(SUCCESS_RESPONSE);
    Serial.print(request_id);
    Serial.print("|");
    Serial.print(message);
    if (finish) Serial.println("");
  }

  void send_failure_response(int16_t request_id);

  template <typename Message>
  void send_failure_response(int16_t request_id, Message message, bool finish = true) {
    Serial.print(FAILURE_RESPONSE);
    Serial.print(request_id);
    Serial.print("|");
    Serial.print(message);
    if (finish) Serial.println("");
  }
};
