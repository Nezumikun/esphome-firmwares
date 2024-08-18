#include "color.h"
#include "math.h"

namespace esphome {
  namespace Nezumikun {
    RGB hsv_to_rgb(HSV hsv) {
      RGB out = { 0, 0, 0 };
      double      hh, p, q, t, ff;
      long        i;
      if (hsv.saturation <= 0.0) {       // < is bogus, just shuts up warnings
          out.red = hsv.value;
          out.green = hsv.value;
          out.blue = hsv.value;
          return out;
      }
      hh = (double)hsv.hue * 360 / 256;
      if(hh >= 360.0) hh = 0.0;
      hh /= 60.0;
      i = (long)hh;
      ff = hh - i;
      p = hsv.value * (1.0 - hsv.saturation);
      q = hsv.value * (1.0 - (hsv.saturation * ff));
      t = hsv.value * (1.0 - (hsv.saturation * (1.0 - ff)));
      switch(i) {
      case 0:
          out.red = hsv.value;
          out.green = t;
          out.blue = p;
          break;
      case 1:
          out.red = q;
          out.green = hsv.value;
          out.blue = p;
          break;
      case 2:
          out.red = p;
          out.green = hsv.value;
          out.blue = t;
          break;
      case 3:
          out.red = p;
          out.green = q;
          out.blue = hsv.value;
          break;
      case 4:
          out.red = t;
          out.green = p;
          out.blue = hsv.value;
          break;
      case 5:
      default:
          out.red = hsv.value;
          out.green = p;
          out.blue = q;
          break;
      }
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