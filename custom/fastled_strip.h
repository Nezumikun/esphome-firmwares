#pragma once

#include "FastLED.h"

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

inline Color ColorFromCRGB(const CRGB &fastled_color) {
  Color result;
  result.r = fastled_color.r;
  result.g = fastled_color.g;
  result.b = fastled_color.b;
  return result;
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

namespace Nezumikun {
  enum PropertyAction { Get, Set, Inc, Dec };
  class FastLedStrip {
    protected:
      static inline CRGB* buffer(CRGB* new_buffer) {
        static CRGB *buffer = NULL;
        if (new_buffer != NULL) {
          buffer = new_buffer;
        }
        return buffer;
      }

      static inline uint8_t hue(PropertyAction action, uint8_t value) {
        static uint8_t _value = 0;
        switch (action) {
          case PropertyAction::Set:
            _value = value;
            break;
          case PropertyAction::Inc:
            _value += value;
            break;
          case PropertyAction::Dec:
            _value -= value;
            break;
          default:
            break;
        }
        return _value;
      }

      static inline CRGB* init_fastled_buffer(uint16_t size) {
        CRGB *temp = buffer(NULL);
        if (temp == NULL) {
          ESP_LOGD("FastLedStrip", "Initial buffer (size %d)", size);
          temp = buffer(new CRGB[size]());
          fill_solid(temp, size, CRGB::Black);
        }
        return temp;
      }

      static inline void copy_fastled_buffer(AddressableLight &it) {
        CRGB *temp = buffer(NULL);
        for (uint16_t i = 0; i < it.size(); i++) {
          it[i] = ColorFromCRGB(temp[i]);
        }
        hue(PropertyAction::Inc, 1);
      }

    public:
      static void rainbow(AddressableLight &it, bool initial_run) {
        static bool withGlitter = false;
        static uint8_t step = 0;
        if (step == 0) {
          step = 255 / it.size();
          if (step == 0) step = 1;
        }
        CRGB *temp = init_fastled_buffer(it.size());
        if (initial_run) {
          withGlitter = random8() & 1;
          ESP_LOGD("fastled_rainbow", "Glitter %s", withGlitter ? "ON" : "OFF");
        }
        fill_rainbow(temp, it.size(), hue(PropertyAction::Get, 0), step);
        if(withGlitter && (random8() < 60)) {
          temp[random16(it.size())] += CRGB::White;
        }
        copy_fastled_buffer(it);
        EVERY_N_SECONDS(20) {
          withGlitter = random8() & 1;
          ESP_LOGD("fastled_rainbow", "Glitter %s", withGlitter ? "ON" : "OFF");
        }
      }

      static void confetti(AddressableLight &it, bool initial_run) {
        // random colored speckles that blink in and fade smoothly
        static uint8_t fadeValue = 0;
        if (fadeValue == 0) {
          fadeValue = 10 * 64 / it.size();
          if (fadeValue == 0) fadeValue = 1;
          ESP_LOGD("fastled_confetti", "Fade value = %d", fadeValue);
        }
        CRGB *temp = init_fastled_buffer(it.size());
        fadeToBlackBy(temp, it.size(), fadeValue);
        int pos = random16(it.size());
        temp[pos] += CHSV(hue(PropertyAction::Get, 0) + random8(64), 200, 255);
        copy_fastled_buffer(it);
      }

      static void sinelon(AddressableLight &it, bool initial_run) {
        static uint8_t fadeValue = 0;
        if (fadeValue == 0) {
          fadeValue = 20 * 64 / it.size();
          if (fadeValue == 0) fadeValue = 1;
          ESP_LOGD("fastled_sinelon", "Fade value = %d", fadeValue);
        }
        CRGB *temp = init_fastled_buffer(it.size());
        fadeToBlackBy(temp, it.size(), fadeValue);
        int pos = beatsin16(13, 0, it.size() - 1);
        temp[pos] += CHSV(hue(PropertyAction::Get, 0), 255, 192);
        copy_fastled_buffer(it);
      }

      static void beats(AddressableLight &it, bool initial_run) {
        // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
        CRGB *temp = init_fastled_buffer(it.size());
        const uint8_t BeatsPerMinute = 62;
        CRGBPalette16 palette = PartyColors_p;
        uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
        for (int i = 0; i < it.size(); i++) { //9948
          temp[i] = ColorFromPalette(palette, hue(PropertyAction::Get, 0) + (i*2), beat - hue(PropertyAction::Get, 0) +(i*10));
        }
        copy_fastled_buffer(it);
      }

      static void juggle(AddressableLight &it, bool initial_run) {
        // eight colored dots, weaving in and out of sync with each other
        static uint8_t fadeValue = 0;
        if (fadeValue == 0) {
          fadeValue = 10 * 64 / it.size();
          if (fadeValue == 0) fadeValue = 1;
          ESP_LOGD("fastled_juggle", "Fade value = %d", fadeValue);
        }
        CRGB *temp = init_fastled_buffer(it.size());
        fadeToBlackBy(temp, it.size(), fadeValue);
        uint8_t dothue = 0;
        for( int i = 0; i < 8; i++) {
          temp[beatsin16(i + 7, 0, it.size()-1 )] |= CHSV(dothue, 200, 255);
          dothue += 32;
        }
        copy_fastled_buffer(it);
      }

      static void noise(AddressableLight &it, bool initial_run) {
        static uint16_t x = 0;
        static uint16_t y = 0;
        static uint8_t subMode = 0;
        CRGB *temp = init_fastled_buffer(it.size());
        for(uint8_t i = 0; i < it.size(); i++) {
          switch(subMode) {
            case 0:
              temp[i] = ColorFromPalette((CRGBPalette16) xmas_gp, inoise8(i * 40, y));
              break;
            case 1:
              temp[i] = ColorFromPalette(HeatColors_p, inoise8(i * 40, y));
              break;
            case 2:
              temp[i] = ColorFromPalette(PartyColors_p, inoise8(i * 40, y));
              break;
            case 3:
              temp[i] = ColorFromPalette(RainbowColors_p, inoise8(i * 40, y));
              break;
            case 4:
              temp[i] = ColorFromPalette(LavaColors_p, inoise8(i * 40, y));
              break;
            case 5:
              temp[i] = ColorFromPalette(CloudColors_p, inoise8(i * 40, y));
              break;
            case 6:
              temp[i] = ColorFromPalette((CRGBPalette16) aurora_gp, inoise8(i * 40, y));
              break;
          }
        }
        x++;
        y += 10;
        copy_fastled_buffer(it);
        EVERY_N_SECONDS(20) { subMode = (subMode + 1) % 7; }
      }

      static void demo(AddressableLight &it, bool initial_run) {
        static uint8_t effectIndex;
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
        EVERY_N_SECONDS(20) { effectIndex = (effectIndex + 1) % 6; }
      }
  };
}

