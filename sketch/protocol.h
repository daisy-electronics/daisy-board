#pragma once

#include <Arduino.h>
#include "serial_mutex.h";
#include "packet.h"

namespace Protocol {
  template <typename Subject>
  void emit_event(Subject subject) {
    SerialMutex sm;
    Serial.print(EVENT);
    Serial.println(subject);
  }

  template <typename Subject, typename Message>
  void emit_event(Subject subject, Message message) {
    SerialMutex sm;

    Serial.print(EVENT);
    Serial.print(subject);
    Serial.print("|");
    Serial.println(message);
  }

  template <typename Subject>
  void send_request(uint32_t request_id, Subject subject) {
    SerialMutex sm;
    Serial.print(REQUEST);
    Serial.print(request_id);
    Serial.print("|");
    Serial.println(subject);
  }

  template <typename Subject, typename Message>
  void send_request(uint32_t request_id, Subject subject, Message message) {
    SerialMutex sm;
    Serial.print(REQUEST);
    Serial.print(request_id);
    Serial.print("|");
    Serial.print(subject);
    Serial.print("|");
    Serial.println(message);
  }

  void send_success_response(uint32_t request_id);

  template <typename Message>
  void send_success_response(uint32_t request_id, Message message) {
    SerialMutex sm;
    Serial.print(SUCCESS_RESPONSE);
    Serial.print(request_id);
    Serial.print("|");
    Serial.println(message);
  }

  void send_failure_response(uint32_t request_id);

  template <typename Message>
  void send_failure_response(uint32_t request_id, Message message) {
    SerialMutex sm;
    Serial.print(FAILURE_RESPONSE);
    Serial.print(request_id);
    Serial.print("|");
    Serial.println(message);
  }
};
