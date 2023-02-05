#include "fastled_strip.h"

namespace esphome {
  namespace Nezumikun {

    void FastLedStripComponent::init_fastled_buffer(uint16_t size) {
      if (buffer == NULL) {
        ESP_LOGD("FastLedStrip", "Initial buffer (size %d)", size);
        buffer = new CRGB[size]();
        fill_solid(buffer, size, CRGB::Black);
      }
    }

    void FastLedStripComponent::copy_fastled_buffer(AddressableLight &it) {
      for (uint16_t i = 0; i < it.size(); i++) {
        it[i] = ColorFromCRGB(buffer[i]);
      }
      hue++;
    }

    void FastLedStripComponent::rainbow(AddressableLight &it, bool initial_run) {
      static bool withGlitter = false;
      static uint8_t step = 0;
      if (step == 0) {
        step = 255 / it.size();
        if (step == 0) step = 1;
      }
      init_fastled_buffer(it.size());
      if (initial_run) {
        withGlitter = random8() & 1;
        ESP_LOGD("fastled_rainbow", "Glitter %s", withGlitter ? "ON" : "OFF");
      }
      fill_rainbow(buffer, it.size(), hue, step);
      if(withGlitter && (random8() < 60)) {
        buffer[random16(it.size())] += CRGB::White;
      }
      copy_fastled_buffer(it);
      EVERY_N_SECONDS(20) {
        withGlitter = random8() & 1;
        ESP_LOGD("fastled_rainbow", "Glitter %s", withGlitter ? "ON" : "OFF");
      }
    }

    void FastLedStripComponent::confetti(AddressableLight &it, bool initial_run) {
      // random colored speckles that blink in and fade smoothly
      static uint8_t fadeValue = 0;
      if (fadeValue == 0) {
        fadeValue = 10 * 64 / it.size();
        if (fadeValue == 0) fadeValue = 1;
        ESP_LOGD("fastled_confetti", "Fade value = %d", fadeValue);
      }
      init_fastled_buffer(it.size());
      fadeToBlackBy(buffer, it.size(), fadeValue);
      int pos = random16(it.size());
      buffer[pos] += CHSV(hue + random8(64), 200, 255);
      copy_fastled_buffer(it);
    }

    void FastLedStripComponent::sinelon(AddressableLight &it, bool initial_run) {
      static uint8_t fadeValue = 0;
      if (fadeValue == 0) {
        fadeValue = 20 * 64 / it.size();
        if (fadeValue == 0) fadeValue = 1;
        ESP_LOGD("fastled_sinelon", "Fade value = %d", fadeValue);
      }
      init_fastled_buffer(it.size());
      fadeToBlackBy(buffer, it.size(), fadeValue);
      int pos = beatsin16(13, 0, it.size() - 1);
      buffer[pos] += CHSV(hue, 255, 192);
      copy_fastled_buffer(it);
    }

    void FastLedStripComponent::beats(AddressableLight &it, bool initial_run) {
      // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
      static uint8_t koefficient = 0;
      if (koefficient == 0) {
        koefficient = 10 * it.size() / 64;
        if (koefficient == 0) koefficient = 1;
        ESP_LOGD("fastled_beats", "koefficient = %d", koefficient);
      }
      init_fastled_buffer(it.size());
      const uint8_t BeatsPerMinute = 62;
      CRGBPalette16 palette = PartyColors_p;
      uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
      for (int i = 0; i < it.size(); i++) { //9948
        buffer[i] = ColorFromPalette(palette, hue + (i*2), beat - hue +(i*koefficient));
      }
      copy_fastled_buffer(it);
    }

    void FastLedStripComponent::juggle(AddressableLight &it, bool initial_run) {
      // eight colored dots, weaving in and out of sync with each other
      static uint8_t fadeValue = 0;
      if (fadeValue == 0) {
        fadeValue = 10 * 64 / it.size();
        if (fadeValue == 0) fadeValue = 1;
        ESP_LOGD("fastled_juggle", "Fade value = %d", fadeValue);
      }
      init_fastled_buffer(it.size());
      fadeToBlackBy(buffer, it.size(), fadeValue);
      uint8_t dothue = 0;
      for( int i = 0; i < 8; i++) {
        buffer[beatsin16(i + 7, 0, it.size()-1 )] |= CHSV(dothue, 200, 255);
        dothue += 32;
      }
      copy_fastled_buffer(it);
    }

    DEFINE_GRADIENT_PALETTE ( aurora_gp ) {
      0,  17, 177,  13,    //Greenish
      64, 121, 242,   5,    //Greenish
      128,  25, 173, 121,    //Turquoise
      192, 250,  77, 127,    //Pink
      255, 171, 101, 221     //Purple
    };

    DEFINE_GRADIENT_PALETTE( xmas_gp ) {
      0,   0, 12,  0,
      40,   0, 55,  0,
      66,   1, 117,  2,
      77,   1, 84,  1,
      81,   0, 55,  0,
      119,   0, 12,  0,
      153,  42,  0,  0,
      181, 121,  0,  0,
      204, 255, 12,  8,
      224, 121,  0,  0,
      244,  42,  0,  0,
      255,  42,  0,  0
    };

    void FastLedStripComponent::noise(AddressableLight &it, bool initial_run) {
      static uint16_t x = 0;
      static uint16_t y = 0;
      static uint8_t subMode = 0;
      EVERY_N_SECONDS(20) {
        subMode = (subMode + 1) % 7;
        initial_run = true;
      }
      if (initial_run) {
        ESP_LOGD("fastled_noise", "Submode = %s", subMode == 0 ? "XMas" : subMode == 1 ? "Heat" : subMode == 2 ? "Party"
          : subMode == 3 ? "Rainbow" : subMode == 4 ? "Lava" : subMode == 5 ? "Cloud" : subMode == 6 ? "Aurora"
          : "Unknown"
        );
      }
      init_fastled_buffer(it.size());
      for(uint16_t i = 0; i < it.size(); i++) {
        switch(subMode) {
          case 0:
            buffer[i] = ColorFromPalette((CRGBPalette16) xmas_gp, inoise8(i * 40, y));
            break;
          case 1:
            buffer[i] = ColorFromPalette(HeatColors_p, inoise8(i * 40, y));
            break;
          case 2:
            buffer[i] = ColorFromPalette(PartyColors_p, inoise8(i * 40, y));
            break;
          case 3:
            buffer[i] = ColorFromPalette(RainbowColors_p, inoise8(i * 40, y));
            break;
          case 4:
            buffer[i] = ColorFromPalette(LavaColors_p, inoise8(i * 40, y));
            break;
          case 5:
            buffer[i] = ColorFromPalette(CloudColors_p, inoise8(i * 40, y));
            break;
          case 6:
            buffer[i] = ColorFromPalette((CRGBPalette16) aurora_gp, inoise8(i * 40, y));
            break;
        }
      }
      x++;
      y += 10;
      copy_fastled_buffer(it);
    }

    void FastLedStripComponent::test(AddressableLight &it, bool initial_run) {
      static uint16_t value = 0;
      static uint8_t step = 0;
      const uint8_t inc = 3;
      init_fastled_buffer(it.size());
      switch (step) {
        case 0:
          fill_solid(buffer, it.size(), CRGB(value, 0, 0));
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
          fill_solid(buffer, it.size(), CRGB(value, 0, 0));
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
          fill_solid(buffer, it.size(), CRGB(0, value, 0));
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
          fill_solid(buffer, it.size(), CRGB(0, value, 0));
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
          fill_solid(buffer, it.size(), CRGB(0, 0, value));
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
          fill_solid(buffer, it.size(), CRGB(0, 0, value));
          if (value == 0) {
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
      copy_fastled_buffer(it);
    }

    void FastLedStripComponent::demo(AddressableLight &it, bool initial_run) {
      static uint8_t effectIndex;
      const char* effectNames[] = {
        "Rainbow", "Confetti", "Sinelon", "Beats", "Juggle", "Noise"
      };
      if (initial_run) {
        firstPass = true;
      }
      if (!testComplited) {
        test(it, initial_run);
        return;
      }
      EVERY_N_SECONDS(20) { 
        uint8_t prevIndex = effectIndex;
        if (firstPass) {
          effectIndex = (effectIndex + 1) % 6;
            if (it.size() == 300) {
              if (effectIndex == 3) {
                effectIndex = 4;
              }
            }
          if (effectIndex == 0) {
            firstPass = false;
            effectIndex = prevIndex;
          } else {
            ESP_LOGD("fastled_demo", "Start effect [%d] %s in first pass", effectIndex, effectNames[effectIndex]);
          }
        }
        if (!firstPass) {
          if ((random8() & 1) == 1) {
            effectIndex = random8(5);
            if (it.size() == 300) {
              if (effectIndex == 3) {
                effectIndex = 5;
              }
            }
            if (prevIndex != effectIndex) {
              ESP_LOGD("fastled_demo", "Start effect [%d] %s", effectIndex, effectNames[effectIndex]);
            }
          }
        }
      }
      switch (effectIndex) {
        case 0:
          rainbow(it, initial_run);
          break;
        case 1:
          confetti(it, initial_run);
          break;
        case 2:
          sinelon(it, initial_run);
          break;
        case 3:
          beats(it, initial_run);
          break;
        case 4:
          juggle(it, initial_run);
          break;
        case 5:
          noise(it, initial_run);
          break;
        default:
          break;
      }
    }

  }
}