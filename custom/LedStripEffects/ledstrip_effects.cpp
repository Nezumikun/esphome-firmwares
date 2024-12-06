#include "ledstrip_effects.h"
#include "math.h"

namespace esphome {
  namespace Nezumikun {
    void LedStripEffectsComponent::init_buffer(uint16_t size) {
      if (buffer == NULL) {
        ESP_LOGD("FastLedStrip", "Initial buffer (size %d)", size);
        buffer = new color[size]();
        fill_solid_rgb(0, 0, 0, size);
      }
    }

    void LedStripEffectsComponent::copy_buffer_rgb(esphome::light::AddressableLight &it) {
      for (uint16_t i = 0; i < it.size(); i++) {
        it[i].set_rgb(buffer[i].rgb.red, buffer[i].rgb.green, buffer[i].rgb.blue);
      }
      hue++;
    }

    void LedStripEffectsComponent::copy_buffer_hsv(esphome::light::AddressableLight &it) {
      for (uint16_t i = 0; i < it.size(); i++) {
        RGB temp = hsv_to_rgb(buffer[i].hsv);
        it[i].set_rgb(temp.red, temp.green, temp.blue);
      }
      hue++;
    }

    void LedStripEffectsComponent::fill_solid_rgb(uint8_t red, uint8_t green, uint8_t blue, uint16_t size) {
      for (int i = 0; i < size; i++) {
        buffer[i].rgb.red = red;
        buffer[i].rgb.green = green;
        buffer[i].rgb.blue = blue;
      }
    }

    void LedStripEffectsComponent::fade_to_black_hsv(uint8_t step, uint16_t size) {
      for (int i = 0; i < size; i++) {
        if (buffer[i].hsv.value <= step) {
          buffer[i].hsv.value = 0;
        } else {
          buffer[i].hsv.value -= step;
        }
      }
    }

    void LedStripEffectsComponent::fade_to_black_rgb(uint8_t percent, uint16_t size) {
      for (int i = 0; i < size; i++) {
        uint8_t delta_r = (uint8_t)((uint16_t)buffer[i].rgb.red * percent / 100);
        if (delta_r == 0) delta_r = 1;
        uint8_t delta_g = (uint8_t)((uint16_t)buffer[i].rgb.green * percent / 100);
        if (delta_g == 0) delta_g = 1;
        uint8_t delta_b = (uint8_t)((uint16_t)buffer[i].rgb.blue * percent / 100);
        if (delta_b == 0) delta_b = 1;
        buffer[i].rgb.red -= delta_r;
        buffer[i].rgb.green -= delta_g;
        buffer[i].rgb.blue -= delta_b;
      }
    }

    void LedStripEffectsComponent::rainbow(esphome::light::AddressableLight &it, bool initial_run) {
      static uint8_t flag = 0;
      static uint16_t step = 0;
      static uint32_t prevTime = -20 * 1000;
      uint32_t now = millis();
      if (prevTime + 20 * 1000 <= now) {
        uint8_t newFlag = random8();
        if (flag != newFlag) {
          flag = newFlag;
          ESP_LOGD("rainbow", "Glitter = %s, Direction = %s", ((flag & 1) == 1) ? "ON" : "OFF", ((flag & 2) == 0) ? "FORWARD" : "BACKWARD");
        }
        prevTime = now;
      }
      if (step == 0) {
        step = 255 * 255 / it.size();
        ESP_LOGD("rainbow", "Step = %i", step);
      }
      if ((flag & 2) > 0) {
        hue -= 2;
      }
      for (int i = 0; i < it.size(); i++) {
        set_color_from_wheel_to_rgb(&(buffer[i].rgb), hue + (step * i / 256), 255);
      }
      if (((flag & 1) == 1) && random8() < 60) {
        uint16_t glitterIndex = random16(it.size() - 1);
        buffer[glitterIndex].rgb.red = 255;
        buffer[glitterIndex].rgb.green = 255;
        buffer[glitterIndex].rgb.blue = 255;
      }
      copy_buffer_rgb(it);
    }

    void LedStripEffectsComponent::color_cycle(esphome::light::AddressableLight &it, bool initial_run) {
      static bool withGlitter = false;
      static uint16_t step = 0;
      static uint8_t bright = 255;
      static bool fade = true;
      static uint32_t prevTime = -20 * 1000;
      uint32_t now = millis();
      if (prevTime + 20 * 1000 <= now) {
        if (withGlitter != (random8() & 1)) {
          withGlitter = !withGlitter;
          ESP_LOGD("color_cycle", "Breath %s", withGlitter ? "ON" : "OFF");
          if (!withGlitter) {
            bright = 255;
            fade = true;
          }
        }
        prevTime = now;
      }
      if (step == 0) {
        step = 1;
        ESP_LOGD("color_cycle", "Start");
      }
      for (int i = 0; i < it.size(); i++) {
        buffer[i].hsv.hue = hue;
        buffer[i].hsv.saturation = 255;
        buffer[i].hsv.value = withGlitter ? bright : 255;
      }
      if (fade) {
        bright -= 1;
        if (bright == 0) fade = false;
      } else {
        bright += 1;
        if (bright == 255) fade = true;
      }
      copy_buffer_hsv(it);
    }

    void LedStripEffectsComponent::confetti(esphome::light::AddressableLight &it, bool initial_run) {
      static uint8_t fadeValue = 0;
      if (fadeValue == 0) {
        fadeValue = 5 * 64 / it.size();
        if (fadeValue == 0) fadeValue = 1;
        ESP_LOGD("confetti", "Fade value = %d", fadeValue);
      }
      fade_to_black_hsv(fadeValue, it.size());
      int pos = random16(it.size() - 1);
      buffer[pos].hsv.hue = hue + random8(64);
      buffer[pos].hsv.saturation = 255;
      buffer[pos].hsv.value = 255;
      copy_buffer_hsv(it);
    }

    void LedStripEffectsComponent::sinelon(esphome::light::AddressableLight &it, bool initial_run) {
      static uint8_t fadeValue = 0;
      if (fadeValue == 0) {
        fadeValue = 12 * 64 / it.size();
        if (fadeValue == 0) fadeValue = 1;
        ESP_LOGD("sinelon", "Fade value = %d", fadeValue);
      }
      fade_to_black_hsv(fadeValue, it.size());
      int pos = beatsin16(13, 0, it.size() - 1);
      buffer[pos].hsv.hue = hue;
      buffer[pos].hsv.saturation = 255;
      buffer[pos].hsv.value = 192;
      copy_buffer_hsv(it);
    }

    void LedStripEffectsComponent::beats(esphome::light::AddressableLight &it, bool initial_run) {
      // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
      static uint8_t koefficient = 0;
      if (koefficient == 0) {
        koefficient = 10 * it.size() / 64;
        if (koefficient == 0) koefficient = 1;
        ESP_LOGD("beats", "koefficient = %d", koefficient);
      }
      const uint8_t BeatsPerMinute = 62;
      uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
      for (int i = 0; i < it.size(); i++) { //9948
        buffer[i].rgb = color_from_palette(palettePartyColors, hue + (i*2), it.size(), beat - hue +(i*koefficient));
      }
      copy_buffer_rgb(it);
    }


    void LedStripEffectsComponent::juggle(esphome::light::AddressableLight &it, bool initial_run) {
      // eight colored dots, weaving in and out of sync with each other
      static uint8_t fadeValue = 0;
      if (fadeValue == 0) {
        fadeValue = 5 * 64 / it.size();
        if (fadeValue == 0) fadeValue = 1;
        ESP_LOGD("fastled_juggle", "Fade value = %d", fadeValue);
      }
      fade_to_black_rgb(fadeValue, it.size());
      uint8_t dothue = 0;
      for( int i = 0; i < 8; i++) {
        uint16_t index = beatsin16(i + 7, 0, it.size() - 1);
        HSV hsv = { .hue = dothue, .saturation = 200, .value = 255 };
        RGB rgb = hsv_to_rgb(hsv);
        if (buffer[index].rgb.red < rgb.red) buffer[index].rgb.red = rgb.red;
        if (buffer[index].rgb.green < rgb.green) buffer[index].rgb.green = rgb.green;
        if (buffer[index].rgb.blue < rgb.blue) buffer[index].rgb.blue = rgb.blue;
        dothue += 32;
      }
      copy_buffer_rgb(it);
    }

    // DEFINE_GRADIENT_PALETTE ( aurora_gp ) {
    //   0,  17, 177,  13,    //Greenish
    //   64, 121, 242,   5,    //Greenish
    //   128,  25, 173, 121,    //Turquoise
    //   192, 250,  77, 127,    //Pink
    //   255, 171, 101, 221     //Purple
    // };

    // DEFINE_GRADIENT_PALETTE( xmas_gp ) {
    //   0,   0, 12,  0,
    //   40,   0, 55,  0,
    //   66,   1, 117,  2,
    //   77,   1, 84,  1,
    //   81,   0, 55,  0,
    //   119,   0, 12,  0,
    //   153,  42,  0,  0,
    //   181, 121,  0,  0,
    //   204, 255, 12,  8,
    //   224, 121,  0,  0,
    //   244,  42,  0,  0,
    //   255,  42,  0,  0
    // };

    void LedStripEffectsComponent::noise(esphome::light::AddressableLight &it, bool initial_run) {
      static uint16_t x = 0;
      static uint16_t y = 0;
      static uint8_t subMode = 0;
      static uint32_t prevTime = 0;
      uint32_t now = millis();
      if (initial_run) prevTime = now;
      if (prevTime + 20 * 1000 <= now) {
        if ((random_uint32() & 1) == 1) {
          subMode = random8(4);
          initial_run = true;
        }
        prevTime = now;
      }
      if (initial_run) {
        ESP_LOGD("noise", "Submode = %s", subMode == 0 ? "Party" : subMode == 1 ? "Heat" : subMode == 2 ? "Cloud"
          : subMode == 3 ? "Rainbow" : subMode == 4 ? "Lava" : subMode == 5 ? "XMas" : subMode == 6 ? "Aurora"
          : "Unknown"
        );
      }
      for(uint16_t i = 0; i < it.size(); i++) {
        switch(subMode) {
          case 0:
            buffer[i].rgb = color_from_palette(palettePartyColors, inoise8(i * 40, y), it.size());
            break;
          case 1:
            buffer[i].rgb = color_from_palette(paletteHeatColors, inoise8(i * 40, y), it.size());
            break;
          case 2:
            buffer[i].rgb = color_from_palette(paletteCloudColors, inoise8(i * 40, y), it.size());
            break;
          case 3:
            buffer[i].rgb = color_from_palette(paletteRainbowColors, inoise8(i * 40, y), it.size());
            break;
          case 4:
            buffer[i].rgb = color_from_palette(paletteLavaColors, inoise8(i * 40, y), it.size());
            break;
          // case 5:
          //   buffer[i] = ColorFromPalette((CRGBPalette16) xmas_gp, inoise8(i * 40, y));
          //   break;
          // case 6:
          //   buffer[i] = ColorFromPalette((CRGBPalette16) aurora_gp, inoise8(i * 40, y));
          //   break;
        }
      }
      x++;
      y += 10;
      copy_buffer_rgb(it);
    }

     void LedStripEffectsComponent::test(esphome::light::AddressableLight &it, bool initial_run) {
      static uint16_t value = 0;
      static uint8_t step = 0;
      const uint8_t inc = 3;

      switch (step) {
        case 0:
          fill_solid_rgb(value, 0, 0, it.size());
          if (value == 255) {
            step++;
          } else {
            value += inc;
            if (value > 255) {
              value = 255;
            }
          }
          break;
        case 1:
          fill_solid_rgb(value, 0, 0, it.size());
          if (value == 0) {
            step++;
          } else {
            if (value < inc) {
              value = 0;
            } else {
              value -= inc;
            }
          }
          break;
        case 2:
          fill_solid_rgb(0, value, 0, it.size());
          if (value == 255) {
            step++;
          } else {
            value += inc;
            if (value > 255) {
              value = 255;
            }
          }
          break;
        case 3:
          fill_solid_rgb(0, value, 0, it.size());
          if (value == 0) {
            step++;
          } else {
            if (value < inc) {
              value = 0;
            } else {
              value -= inc;
            }
          }
          break;
        case 4:
          fill_solid_rgb(0, 0, value, it.size());
          if (value == 255) {
            step++;
          } else {
            value += inc;
            if (value > 255) {
              value = 255;
            }
          }
          break;
        case 5:
          fill_solid_rgb(0, 0, value, it.size());
          if (value == 0) {
            ESP_LOGD("test", "Test complited");
            testComplited = true;
          } else {
            if (value < inc) {
              value = 0;
            } else {
              value -= inc;
            }
          }
          break;
      }
      copy_buffer_rgb(it);
    }

    void LedStripEffectsComponent::demo(esphome::light::AddressableLight &it, bool initial_run) {
      static uint8_t effectIndex = -1;
      const char* effectNames[] = {
        "Rainbow", "Confetti", "Juggle", "Color Cycle", "Sinelon", "Beats", "Noise"
      };
      init_buffer(it.size());
      if (initial_run) {
        ESP_LOGD("demo", "First pass started");
        firstPass = true;
      }
      if (!testComplited) {
        test(it, initial_run);
        return;
      }
      static uint32_t prevTime = -20 * 1000;
      uint32_t now = millis();
      if (prevTime + 20 * 1000 <= now) {
        uint8_t prevIndex = effectIndex;
        uint8_t limit = (it.size() > 100) ? 6 : 6;
        if (firstPass) {
          effectIndex++;
          if (effectIndex > limit) {
            firstPass = false;
            effectIndex--;
            ESP_LOGD("demo", "First pass finished");
          } else {
            ESP_LOGD("demo", "Start effect [%d] %s in first pass", effectIndex, effectNames[effectIndex]);
          }
        }
        if (!firstPass) {
          if ((random8() & 1) == 1) {
            effectIndex = random8(limit - 1);
            if (prevIndex != effectIndex) {
              ESP_LOGD("demo", "Start effect [%d] %s", effectIndex, effectNames[effectIndex]);
            }
          }
        }
        prevTime = now;
      }
      switch (effectIndex) {
        case 0:
          rainbow(it, initial_run);
          break;
        case 1:
          confetti(it, initial_run);
          break;
        case 2:
          juggle(it, initial_run);
          break;
        case 3:
          color_cycle(it, initial_run);
          break;
        case 4:
          sinelon(it, initial_run);
          break;
        case 5:
          beats(it, initial_run);
          break;
        case 6:
          noise(it, initial_run);
          break;
        default:
          break;
      }
    }

  }
}