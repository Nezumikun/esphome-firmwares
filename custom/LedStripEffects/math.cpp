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

    uint8_t qadd8( uint8_t i, uint8_t j)
    {
      unsigned int t = i + j;
      if( t > 255) t = 255;
      return t;
    }

    uint8_t const P[] = {
      151, 160, 137,  91,  90,  15, 131,  13, 201,  95,  96,  53, 194, 233,   7, 225,
      140,  36, 103,  30,  69, 142,   8,  99,  37, 240,  21,  10,  23, 190,   6, 148,
      247, 120, 234,  75,   0,  26, 197,  62,  94, 252, 219, 203, 117,  35,  11,  32,
      57, 177,  33,  88, 237, 149,  56,  87, 174,  20, 125, 136, 171, 168,  68, 175,
      74, 165,  71, 134, 139,  48,  27, 166,  77, 146, 158, 231,  83, 111, 229, 122,
      60, 211, 133, 230, 220, 105,  92,  41,  55,  46, 245,  40, 244, 102, 143,  54,
      65,  25,  63, 161,   1, 216,  80,  73, 209,  76, 132, 187, 208,  89,  18, 169,
      200, 196, 135, 130, 116, 188, 159,  86, 164, 100, 109, 198, 173, 186,   3,  64,
      52, 217, 226, 250, 124, 123,   5, 202,  38, 147, 118, 126, 255,  82,  85, 212,
      207, 206,  59, 227,  47,  16,  58,  17, 182, 189,  28,  42, 223, 183, 170, 213,
      119, 248, 152,   2,  44, 154, 163,  70, 221, 153, 101, 155, 167,  43, 172,   9,
      129,  22,  39, 253,  19,  98, 108, 110,  79, 113, 224, 232, 178, 185, 112, 104,
      218, 246,  97, 228, 251,  34, 242, 193, 238, 210, 144,  12, 191, 179, 162, 241,
      81,  51, 145, 235, 249,  14, 239, 107,  49, 192, 214,  31, 181, 199, 106, 157,
      184,  84, 204, 176, 115, 121,  50,  45, 127,   4, 150, 254, 138, 236, 205,  93,
      222, 114,  67,  29,  24,  72, 243, 141, 128, 195,  78,  66, 215,  61, 156, 180,
      151
    };

    uint8_t ease8InOutQuad(uint8_t i) {
      uint8_t j = i;
      if( j & 0x80 ) {
          j = 255 - j;
      }
      uint8_t jj  = scale8(  j, j);
      uint8_t jj2 = jj << 1;
      if( i & 0x80 ) {
          jj2 = 255 - jj2;
      }
      return jj2;
    }

    int8_t avg7(int8_t i, int8_t j) {
      return (i>>1) + (j>>1) + (i & 0x1);
    }

    int8_t grad8(uint8_t hash, int8_t x, int8_t y) {
      int8_t u,v;
      if( hash & 4) {
          u = y; v = x;
      } else {
          u = x; v = y;
      }
  
      if(hash&1) { u = -u; }
      if(hash&2) { v = -v; }
  
      return avg7(u,v);
    }

    int8_t lerp7by8(int8_t a, int8_t b, uint8_t frac) {
        int8_t result;
        if( b > a) {
            uint8_t delta = b - a;
            uint8_t scaled = scale8( delta, frac);
            result = a + scaled;
        } else {
            uint8_t delta = a - b;
            uint8_t scaled = scale8( delta, frac);
            result = a - scaled;
        }
        return result;
    }

    int8_t inoise8_raw(uint16_t x, uint16_t y)
    {
      // Find the unit cube containing the point
      uint8_t X = x>>8;
      uint8_t Y = y>>8;
  
      // Hash cube corner coordinates
      uint8_t A = P[X]+Y;
      uint8_t AA = P[A];
      uint8_t AB = P[A+1];
      uint8_t B = P[X+1]+Y;
      uint8_t BA = P[B];
      uint8_t BB = P[B+1];
  
      // Get the relative position of the point in the cube
      uint8_t u = x;
      uint8_t v = y;
  
      // Get a signed version of the above for the grad function
      int8_t xx = ((uint8_t)(x)>>1) & 0x7F;
      int8_t yy = ((uint8_t)(y)>>1) & 0x7F;
      uint8_t N = 0x80;
  
      u = ease8InOutQuad(u);
      v = ease8InOutQuad(v);
  
      int8_t X1 = lerp7by8(grad8(P[AA], xx, yy), grad8(P[BA], xx - N, yy), u);
      int8_t X2 = lerp7by8(grad8(P[AB], xx, yy-N), grad8(P[BB], xx - N, yy - N), u);
  
      int8_t ans = lerp7by8(X1,X2,v);
  
      return ans;
      // return scale8((70+(ans)),234)<<1;
    }

    uint8_t inoise8(uint16_t x, uint16_t y) {
      int8_t n = inoise8_raw( x, y);  // -64..+64
      n+= 64;                         //   0..128
      uint8_t ans = qadd8( n, n);     //   0..255
      return ans;
    }
  }
}