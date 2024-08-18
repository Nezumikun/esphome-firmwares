#pragma once

#include "esphome/components/light/addressable_light.h"

namespace esphome {
  namespace Nezumikun {
    uint8_t random8();
    uint8_t random8(uint8_t limit);
    uint16_t random16(uint16_t limit);
    uint16_t beat88(uint16_t beats_per_minute_88, uint32_t timebase = 0);
    uint8_t beat8(uint16_t beats_per_minute, uint32_t timebase = 0);
    uint16_t beat16(uint16_t beats_per_minute, uint32_t timebase = 0);
    uint8_t sin8( uint8_t theta);
    int16_t sin16(uint16_t theta);
    uint8_t scale8( uint8_t i, u_int8_t scale);
    uint16_t scale16( uint16_t i, uint16_t scale );
    uint8_t beatsin8(uint16_t beats_per_minute, uint8_t lowest = 0, uint8_t highest = 255, uint32_t timebase = 0, uint8_t phase_offset = 0);
    uint16_t beatsin16( uint16_t beats_per_minute, uint16_t lowest = 0, uint16_t highest = 65535, uint32_t timebase = 0, uint16_t phase_offset = 0);
    
  }
}