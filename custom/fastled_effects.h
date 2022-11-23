#pragma once

#include "FastLED.h"

inline Color ColorFromCRGB(const CRGB &fastled_color) {
  Color result;
  result.r = fastled_color.r;
  result.g = fastled_color.g;
  result.b = fastled_color.b;
  return result;
}

CRGB *fastled_buffer = NULL;
uint8_t fastled_hue = 0;

inline void init_fastled_buffer(uint8_t size) {
  if (fastled_buffer == NULL) {
    ESP_LOGD("fastled", "Initial buffer (size %d)", size);
    fastled_buffer = new CRGB[size]();
    fill_solid(fastled_buffer, size, CRGB::Black);
  }
}

inline void copy_fastled_buffer(AddressableLight &it) {
  for (uint8_t i = 0; i < it.size(); i++) {
    it[i] = ColorFromCRGB(fastled_buffer[i]);
  }
  fastled_hue++;
}

void fastled_rainbow(AddressableLight &it, bool initial_run) {
  static bool withGlitter = false;
  init_fastled_buffer(it.size());
  if (initial_run) {
    withGlitter = random8() & 1;
    ESP_LOGD("fastled_rainbow", "Glitter %s", withGlitter ? "ON" : "OFF");
  }
  fill_rainbow(fastled_buffer, it.size(), fastled_hue, 255 / it.size());
  if(withGlitter && (random8() < 60)) {
    fastled_buffer[random16(it.size())] += CRGB::White;
  }
  copy_fastled_buffer(it);
}

void fastled_confetti(AddressableLight &it, bool initial_run) {
  // random colored speckles that blink in and fade smoothly
  init_fastled_buffer(it.size());
  fadeToBlackBy(fastled_buffer, it.size(), 10);
  int pos = random16(it.size());
  fastled_buffer[pos] += CHSV(fastled_hue + random8(64), 200, 255);
  copy_fastled_buffer(it);
}

void fastled_sinelon(AddressableLight &it, bool initial_run) {
  init_fastled_buffer(it.size());
  fadeToBlackBy(fastled_buffer, it.size(), 10);
  int pos = beatsin16(13, 0, it.size() - 1);
  fastled_buffer[pos] += CHSV(fastled_hue, 255, 192);
  copy_fastled_buffer(it);
}

void fastled_beats(AddressableLight &it, bool initial_run) {
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  init_fastled_buffer(it.size());
  const uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
  for( int i = 0; i < it.size(); i++) { //9948
    fastled_buffer[i] = ColorFromPalette(palette, fastled_hue + (i*2), beat - fastled_hue +(i*10));
  }
  copy_fastled_buffer(it);
}

void fastled_juggle(AddressableLight &it, bool initial_run) {
  // eight colored dots, weaving in and out of sync with each other
  init_fastled_buffer(it.size());
  fadeToBlackBy(fastled_buffer, it.size(), 10);
  uint8_t dothue = 0;
  for( int i = 0; i < 8; i++) {
    fastled_buffer[beatsin16(i + 7, 0, it.size()-1 )] |= CHSV(dothue, 200, 255);
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
void fastled_noise(AddressableLight &it, bool initial_run) {
  static uint16_t x = 0;
  static uint16_t y = 0;
  static uint8_t subMode = 0;
  init_fastled_buffer(it.size());
  for(uint8_t i = 0; i < it.size(); i++) {
    switch(subMode) {
      case 0:
        fastled_buffer[i] = ColorFromPalette((CRGBPalette16) xmas_gp, inoise8(i * 40, y));
        break;
      case 1:
        fastled_buffer[i] = ColorFromPalette(HeatColors_p, inoise8(i * 40, y));
        break;
      case 2:
        fastled_buffer[i] = ColorFromPalette(PartyColors_p, inoise8(i * 40, y));
        break;
      case 3:
        fastled_buffer[i] = ColorFromPalette(RainbowColors_p, inoise8(i * 40, y));
        break;
      case 4:
        fastled_buffer[i] = ColorFromPalette(LavaColors_p, inoise8(i * 40, y));
        break;
      case 5:
        fastled_buffer[i] = ColorFromPalette(CloudColors_p, inoise8(i * 40, y));
        break;
      case 6:
        fastled_buffer[i] = ColorFromPalette((CRGBPalette16) aurora_gp, inoise8(i * 40, y));
        break;
    }
  }
  x++;
  y += 10;
  copy_fastled_buffer(it);
  EVERY_N_SECONDS(20) { subMode = (subMode + 1) % 7; }
}

typedef void (*SimplePatternList[])(AddressableLight&, bool);
SimplePatternList gPatterns = { fastled_rainbow, fastled_confetti, fastled_sinelon, fastled_juggle, fastled_beats, fastled_noise };
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void fastled_demo(AddressableLight &it, bool initial_run) {
  static uint8_t effectNumber = 0;
  gPatterns[effectNumber](it, initial_run);
  EVERY_N_SECONDS(20) { effectNumber = (effectNumber + 1) % ARRAY_SIZE(gPatterns); }
}
