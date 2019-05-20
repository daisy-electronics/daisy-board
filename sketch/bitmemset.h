#pragma once

#include <Arduino.h>

void bitmemset(uint8_t dest[], uint8_t dest_offset, uint8_t source[],
    uint8_t source_offset, uint8_t length);

void bitmemset_v(uint8_t dest[], uint8_t dest_offset, uint8_t source,
    uint8_t source_offset, uint8_t length);
