#include "protocol.h"

void Protocol::send_success_response(int16_t request_id) {
  Serial.print(SUCCESS_RESPONSE);
  Serial.println(request_id);
}

void Protocol::send_failure_response(int16_t request_id) {
  Serial.print(FAILURE_RESPONSE);
  Serial.println(request_id);
}
