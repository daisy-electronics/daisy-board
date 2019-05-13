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

  template <typename Subject, typename Message1, typename Message2>
  void emit_event(Subject subject, Message1 message1, Message2 message2) {
    SerialMutex sm;

    Serial.print(EVENT);
    Serial.print(subject);
    Serial.print("|");
    Serial.print(message1);
    Serial.print("|");
    Serial.println(message2);
  }

  template <typename Subject, typename Message1, typename Message2, typename Message3>
  void emit_event(Subject subject, Message1 message1, Message2 message2, Message3 message3) {
    SerialMutex sm;

    Serial.print(EVENT);
    Serial.print(subject);
    Serial.print("|");
    Serial.print(message1);
    Serial.print("|");
    Serial.print(message2);
    Serial.print("|");
    Serial.println(message3);
  }

  template <typename Subject>
  void send_request(int16_t request_id, Subject subject) {
    SerialMutex sm;
    Serial.print(REQUEST);
    Serial.print(request_id);
    Serial.print("|");
    Serial.println(subject);
  }

  template <typename Subject, typename Message>
  void send_request(int16_t request_id, Subject subject, Message message) {
    SerialMutex sm;
    Serial.print(REQUEST);
    Serial.print(request_id);
    Serial.print("|");
    Serial.print(subject);
    Serial.print("|");
    Serial.println(message);
  }

  void send_success_response(int16_t request_id);

  template <typename Message>
  void send_success_response(int16_t request_id, Message message) {
    SerialMutex sm;
    Serial.print(SUCCESS_RESPONSE);
    Serial.print(request_id);
    Serial.print("|");
    Serial.println(message);
  }

  template <typename Message1, typename Message2>
  void send_success_response(int16_t request_id, Message1 message1, Message2 message2) {
    SerialMutex sm;
    Serial.print(SUCCESS_RESPONSE);
    Serial.print(request_id);
    Serial.print("|");
    Serial.print(message1);
    Serial.print("|");
    Serial.println(message2);
  }

  void send_failure_response(int16_t request_id);

  template <typename Message>
  void send_failure_response(int16_t request_id, Message message) {
    SerialMutex sm;
    Serial.print(FAILURE_RESPONSE);
    Serial.print(request_id);
    Serial.print("|");
    Serial.println(message);
  }

  template <typename Message1, typename Message2>
  void send_failure_response(int16_t request_id, Message1 message1, Message2 message2) {
    SerialMutex sm;
    Serial.print(FAILURE_RESPONSE);
    Serial.print(request_id);
    Serial.print("|");
    Serial.print(message1);
    Serial.print("|");
    Serial.println(message2);
  }
};
