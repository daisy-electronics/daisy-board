#include "read_string.h"

#define MESSAGE_TOO_LONG F("message too long")

void read_string(char *received_string) {
  size_t i = 0;

  while (Serial.available() > 0) {
    const char ch = Serial.read();
    if (ch == '\n') break;
    if (i < Protocol::MESSAGE_MAX_LENGTH - 1) {
      received_string[i++] = ch;
    } else if (i == Protocol::MESSAGE_MAX_LENGTH - 1) {
      Protocol::emit_event("error", MESSAGE_TOO_LONG);
    }
  }

  received_string[i] = '\0';
}
