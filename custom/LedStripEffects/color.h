#pragma once

#include "esphome/components/light/addressable_light.h"

namespace esphome {
  namespace Nezumikun {

    typedef struct {
      uint8_t red;
      uint8_t green;
      uint8_t blue;
     } RGB;

    typedef struct {
      uint8_t hue;
      uint8_t saturation;
      uint8_t value;
    } HSV;

    typedef union {
      RGB rgb;
      HSV hsv;
    } color;

    RGB hsv_to_rgb(HSV hsv);

    typedef RGB PaletteRGB16[16];
    
    const PaletteRGB16 palettePartyColors = {
      { 0x55, 0x00, 0xAB }, { 0x84, 0x00, 0x7C }, { 0xB5, 0x00, 0x4B }, { 0xE5, 0x00, 0x1B },
      { 0xE8, 0x17, 0x00 }, { 0xB8, 0x47, 0x00 }, { 0xAB, 0x77, 0x00 }, { 0xAB, 0xAB, 0x00 },
      { 0xAB, 0x55, 0x00 }, { 0xDD, 0x22, 0x00 }, { 0xF2, 0x00, 0x0E }, { 0xC2, 0x00, 0x3E },
      { 0x8F, 0x00, 0x71 }, { 0x5F, 0x00, 0xA1 }, { 0x2F, 0x00, 0xD0 }, { 0x00, 0x07, 0xF9 }
    };

    RGB color_from_palette (const PaletteRGB16 pal16, uint8_t index, uint16_t size, uint8_t brightness = 255);

  }
}
