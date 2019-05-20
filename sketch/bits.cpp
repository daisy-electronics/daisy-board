#include "bits.h"

uint8_t Bits::to_byte(uint8_t bits[], uint8_t bits_len) {
  uint8_t byte = 0;
  for (int8_t i = bits_len - 1; i >= 0; i--) {
    bitWrite(byte, bits_len, bits[i]);
  }

  return byte;
}
