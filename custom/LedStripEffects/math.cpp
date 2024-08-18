#include "math.h"

namespace esphome {
  namespace Nezumikun {

    uint8_t random8()
    {
        return random_uint32() & 0xff;
    }

    uint8_t random8(uint8_t limit)
    {
        return random8() % (limit + 1);
    }

    uint16_t random16(uint16_t limit)
    {
        return (random_uint32() & 0xffff) % (limit + 1);
    }

    uint16_t beat88(uint16_t beats_per_minute_88, uint32_t timebase)
    {
        return (((millis()) - timebase) * beats_per_minute_88 * 280) >> 16;
    }
 
    uint16_t beat16(uint16_t beats_per_minute, uint32_t timebase)
    {
        // Convert simple 8-bit BPM's to full Q8.8 accum88's if needed
        if( beats_per_minute < 256) beats_per_minute <<= 8;
        return beat88(beats_per_minute, timebase);
    }

    uint8_t beat8(uint16_t beats_per_minute, uint32_t timebase) {
      return beat16( beats_per_minute, timebase) >> 8;
    }

    int16_t sin16(uint16_t theta)
    {
        static const uint16_t base[] =
        { 0, 6393, 12539, 18204, 23170, 27245, 30273, 32137 };
        static const uint8_t slope[] =
        { 49, 48, 44, 38, 31, 23, 14, 4 };
    
        uint16_t offset = (theta & 0x3FFF) >> 3; // 0..2047
        if( theta & 0x4000 ) offset = 2047 - offset;
    
        uint8_t section = offset / 256; // 0..7
        uint16_t b   = base[section];
        uint8_t  m   = slope[section];
    
        uint8_t secoffset8 = (uint8_t)(offset) / 2;
    
        uint16_t mx = m * secoffset8;
        int16_t  y  = mx + b;
    
        if( theta & 0x8000 ) y = -y;
    
        return y;
    }

    uint8_t sin8( uint8_t theta)
    {
      const uint8_t b_m16_interleave[] = { 0, 49, 49, 41, 90, 27, 117, 10 };
      uint8_t offset = theta;
      if( theta & 0x40 ) {
          offset = (uint8_t)255 - offset;
      }
      offset &= 0x3F; // 0..63
  
      uint8_t secoffset  = offset & 0x0F; // 0..15
      if( theta & 0x40) ++secoffset;
  
      uint8_t section = offset >> 4; // 0..3
      uint8_t s2 = section * 2;
      const uint8_t* p = b_m16_interleave;
      p += s2;
      uint8_t b   =  *p;
      ++p;
      uint8_t m16 =  *p;
  
      uint8_t mx = (m16 * secoffset) >> 4;
  
      int8_t y = mx + b;
      if( theta & 0x80 ) y = -y;
  
      y += 128;
  
      return y;
    }

    uint16_t scale16(uint16_t i, uint16_t scale)
    {
        uint16_t result;
        result = ((uint32_t)(i) * (1 + (uint32_t)(scale))) / 65536;
        return result;
    }

    uint8_t scale8( uint8_t i, u_int8_t scale) {
      return (((uint16_t)i) * (1 + (uint16_t)(scale))) >> 8;
    }

    uint16_t beatsin16(uint16_t beats_per_minute, uint16_t lowest, uint16_t highest, uint32_t timebase, uint16_t phase_offset)
    {
        uint16_t beat = beat16( beats_per_minute, timebase);
        uint16_t beatsin = (sin16( beat + phase_offset) + 32768);
        uint16_t rangewidth = highest - lowest;
        uint16_t scaledbeat = scale16( beatsin, rangewidth);
        uint16_t result = lowest + scaledbeat;
        return result;
    }

    uint8_t beatsin8(uint16_t beats_per_minute, uint8_t lowest, uint8_t highest, uint32_t timebase, uint8_t phase_offset)
    {
        uint8_t beat = beat8( beats_per_minute, timebase);
        uint8_t beatsin = sin8( beat + phase_offset);
        uint8_t rangewidth = highest - lowest;
        uint8_t scaledbeat = scale8( beatsin, rangewidth);
        uint8_t result = lowest + scaledbeat;
        return result;
    }    
  }
}