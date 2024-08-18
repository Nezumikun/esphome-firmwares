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
        it[i].set_hsv(esphome::light::ESPHSVColor(buffer[i].hsv.hue, buffer[i].hsv.saturation, buffer[i].hsv.value));
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
      static bool withGlitter = false;
      static uint16_t step = 0;
      static uint32_t prevTime = -20 * 1000;
      uint32_t now = millis();
      if (prevTime + 20 * 1000 <= now) {
        if (withGlitter != (random8() & 1)) {
          withGlitter = !withGlitter;
          ESP_LOGD("rainbow", "Glitter %s", withGlitter ? "ON" : "OFF");
        }
        prevTime = now;
      }
      if (step == 0) {
        step = 255 * 255 / it.size();
        ESP_LOGD("rainbow", "Step = %i", step);
      }
      for (int i = 0; i < it.size(); i++) {
        buffer[i].hsv.hue = hue + (step * i / 256);
        buffer[i].hsv.saturation = 255;
        buffer[i].hsv.value = 255;
      }
      if(withGlitter && random8() < 60) {
         buffer[random16(it.size())].hsv.hue = 0;
         buffer[random16(it.size())].hsv.saturation = 0;
         buffer[random16(it.size())].hsv.value = 255;
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
        fadeValue = 6 * 64 / it.size();
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

    // void LedStripEffectsComponent::noise(AddressableLight &it, bool initial_run) {
    //   static uint16_t x = 0;
    //   static uint16_t y = 0;
    //   static uint8_t subMode = 0;
    //   EVERY_N_SECONDS(20) {
    //     subMode = (subMode + 1) % 7;
    //     initial_run = true;
    //   }
    //   if (initial_run) {
    //     ESP_LOGD("fastled_noise", "Submode = %s", subMode == 0 ? "XMas" : subMode == 1 ? "Heat" : subMode == 2 ? "Party"
    //       : subMode == 3 ? "Rainbow" : subMode == 4 ? "Lava" : subMode == 5 ? "Cloud" : subMode == 6 ? "Aurora"
    //       : "Unknown"
    //     );
    //   }
    //   init_fastled_buffer(it.size());
    //   for(uint16_t i = 0; i < it.size(); i++) {
    //     switch(subMode) {
    //       case 0:
    //         buffer[i] = ColorFromPalette((CRGBPalette16) xmas_gp, inoise8(i * 40, y));
    //         break;
    //       case 1:
    //         buffer[i] = ColorFromPalette(HeatColors_p, inoise8(i * 40, y));
    //         break;
    //       case 2:
    //         buffer[i] = ColorFromPalette(PartyColors_p, inoise8(i * 40, y));
    //         break;
    //       case 3:
    //         buffer[i] = ColorFromPalette(RainbowColors_p, inoise8(i * 40, y));
    //         break;
    //       case 4:
    //         buffer[i] = ColorFromPalette(LavaColors_p, inoise8(i * 40, y));
    //         break;
    //       case 5:
    //         buffer[i] = ColorFromPalette(CloudColors_p, inoise8(i * 40, y));
    //         break;
    //       case 6:
    //         buffer[i] = ColorFromPalette((CRGBPalette16) aurora_gp, inoise8(i * 40, y));
    //         break;
    //     }
    //   }
    //   x++;
    //   y += 10;
    //   copy_fastled_buffer(it);
    // }

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
        "Rainbow", "Confetti", "Juggle", "Sinelon", "Beats", "Noise"
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
        uint8_t limit = (it.size() > 100) ? 3 : 4;
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
            effectIndex = random8(limit);
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
          sinelon(it, initial_run);
          break;
        case 4:
          beats(it, initial_run);
          break;
        // case 5:
        //   noise(it, initial_run);
        //   break;
        default:
          break;
      }
    }

  }
}