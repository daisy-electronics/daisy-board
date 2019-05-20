#include "bitmemset.h"

void bitmemset(uint8_t dest[], uint8_t dest_offset, uint8_t source[],
    uint8_t source_offset, uint8_t length) {

  for (uint8_t i = 0; i < length; i++) {
    const uint8_t source_byte_i = (source_offset + i) / 8;
    const uint8_t source_bit_i = (source_offset + i) % 8;
    const uint8_t dest_byte_i = (dest_offset + i) / 8;
    const uint8_t dest_bit_i = (dest_offset + i) % 8;

    bitWrite(dest[dest_byte_i], 7 - dest_bit_i,
      bitRead(source[source_byte_i], 7 - source_bit_i));
  }
}

void bitmemset_v(uint8_t dest[], uint8_t dest_offset, uint8_t source,
    uint8_t source_offset, uint8_t length) {

  for (uint8_t i = 0; i < length; i++) {
    const uint8_t source_bit_i = (source_offset + i) % 8;
    const uint8_t dest_byte_i = (dest_offset + i) / 8;
    const uint8_t dest_bit_i = (dest_offset + i) % 8;

    bitWrite(dest[dest_byte_i], 7 - dest_bit_i,
      bitRead(source, 7 - source_bit_i));
  }
}
