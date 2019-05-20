#include "protocol_internal.h"

#include <math.h>
#include "protocol_constants.h"
#include "serial_mutex.h"
#include "bitmemset.h"

namespace {
  void send_response(uint8_t packet_type, uint8_t data[], uint8_t data_len);
  void fill_in_data(uint8_t packet[], uint8_t packet_size, uint8_t offset, uint8_t data[], uint8_t data_len);
};

void Protocol::emit_event(uint8_t subject, uint8_t data[], uint8_t data_len) {
  const uint8_t packet_size = ceil((6 + data_len) / 8.);

  uint8_t packet[packet_size];
  bitmemset_v(packet, 0, EVENT, 6, 2);
  bitmemset_v(packet, 2, subject, 4, 4);
  fill_in_data(packet, packet_size, 6, data, data_len);

  SerialMutex sm;
  Serial.write(packet, packet_size);
}

void Protocol::send_success_response(uint8_t data[], uint8_t data_len) {
  send_response(SUCCESS_RESPONSE, data, data_len);
}

void Protocol::send_failure_response(uint8_t data[], uint8_t data_len) {
  send_response(FAILURE_RESPONSE, data, data_len);
}

namespace {
  void send_response(uint8_t packet_type, uint8_t data[], uint8_t data_len) {
    const uint8_t packet_size = ceil((2 + data_len) / 8.);

    uint8_t packet[packet_size];
    bitmemset_v(packet, 0, packet_type, 6, 2);
    fill_in_data(packet, packet_size, 2, data, data_len);

    SerialMutex sm;
    Serial.write(packet, packet_size);
  }

  void fill_in_data(uint8_t packet[], uint8_t packet_size, uint8_t offset, uint8_t data[], uint8_t data_len) {
    bitmemset(packet, offset, data, 0, data_len);

    // write zero's at the end
    const uint8_t trailing = (offset + data_len - 1) % 8 + 1;
    const uint8_t rest = 8 - trailing;
    for (uint8_t i = 0; i < rest; i++) {
      bitWrite(packet[packet_size - 1], i, 0);
    }
  }
}
