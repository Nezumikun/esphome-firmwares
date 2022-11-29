#pragma once

#include "esphome/core/component.h"
#include "esphome/core/defines.h"
#include "esphome/core/color.h"
#include "esphome/core/log.h"
#include "esphome/components/light/addressable_light.h"

using namespace esphome::light;

#define FASTLED_ESP8266_RAW_PIN_ORDER
#define FASTLED_ESP32_RAW_PIN_ORDER
#define FASTLED_RMT_BUILTIN_DRIVER true

// Avoid annoying compiler messages
#define FASTLED_INTERNAL

#include "FastLED.h"


namespace esphome {
  namespace Nezumikun {

    inline Color ColorFromCRGB(const CRGB &fastled_color) {
      Color result;
      result.r = fastled_color.r;
      result.g = fastled_color.g;
      result.b = fastled_color.b;
      return result;
    }

    class FastLedStrip {
      protected:
        static CRGB *buffer;
        static uint8_t hue;
        static CRGB* init_fastled_buffer(uint16_t size);
        static void copy_fastled_buffer(AddressableLight &it);
      public:
        static void rainbow(AddressableLight &it, bool initial_run);
        static void confetti(AddressableLight &it, bool initial_run);
        static void sinelon(AddressableLight &it, bool initial_run);
        static void beats(AddressableLight &it, bool initial_run);
        static void juggle(AddressableLight &it, bool initial_run);
        static void noise(AddressableLight &it, bool initial_run);
        static void demo(AddressableLight &it, bool initial_run);
    };
  }
}
