#include "color.h"
#include "math.h"

#define fade8(x, b)     (((uint16_t)(x) * ((b) + 1)) >> 8)

namespace esphome {
  namespace Nezumikun {
    void set_color_from_wheel_to_rgb(RGB *rgb, uint8_t color, uint8_t bright) {
      uint8_t r, g, b;
      uint8_t shift;
      if (color > 170) {
          shift = (color - 170) * 3;
          r = shift;
          g = 0;
          b = 255 - shift;
      } else if (color > 85) {
          shift = (color - 85) * 3;
          r = 0;
          g = 255 - shift;
          b = shift;
      } else {
          shift = color * 3;
          r = 255 - shift;
          g = shift;
          b = 0;
      }
      if (bright != 255) {
          r = fade8(r, bright);
          g = fade8(g, bright);
          b = fade8(b, bright);
      }
      rgb->red = r;
      rgb->blue = b;
      rgb->green = g;
    }

    RGB hsv_to_rgb(HSV hsv) {
      RGB out = { 0, 0, 0 };
      uint8_t r, g, b;
      uint8_t color = hsv.hue;
      uint8_t bright = hsv.value;
      uint8_t shift;
      if (color > 170) {
          shift = (color - 170) * 3;
          r = shift;
          g = 0;
          b = 255 - shift;
      } else if (color > 85) {
          shift = (color - 85) * 3;
          r = 0;
          g = 255 - shift;
          b = shift;
      } else {
          shift = color * 3;
          r = 255 - shift;
          g = shift;
          b = 0;
      }
      if (bright != 255) {
          r = fade8(r, bright);
          g = fade8(g, bright);
          b = fade8(b, bright);
      }
      out.red = r;
      out.blue = b;
      out.green = g;
      return out;
    }

    RGB color_from_palette (const PaletteRGB16 pal16, uint8_t index, uint16_t size, uint8_t brightness) {
      uint8_t hi4 = index >> 4;
      uint8_t lo4 = index & 0x0F;
      const RGB* entry = pal16 + hi4;
      uint8_t red = entry->red;
      uint8_t green = entry->green;
      uint8_t blue = entry->blue;
      if (lo4) {
        if (hi4 == 15) {
          entry = pal16;
        } else {
          ++entry;
        }
        uint8_t f2 = lo4 << 4;
        uint8_t f1 = 255 - f2;
        red = esp_scale8(red, f1) + esp_scale8(entry->red, f2);
        green = esp_scale8(green, f1) + esp_scale8(entry->green, f2);
        blue = esp_scale8(blue, f1) + esp_scale8(entry->blue, f2);
      }
      if (brightness != 255) {
        if (brightness) {
          ++brightness;
          if (red) {
            red = esp_scale8(red, brightness);
            ++red;
          }
          if (green) {
            green = esp_scale8(green, brightness);
            ++green;
          }
          if (blue) {
            blue = esp_scale8(blue, brightness);
            ++blue;
          }
        } else {
          red = 0;
          green = 0;
          blue = 0;
        }
      }
      RGB out = { red, green, blue };
      return out;
    }

  }
}