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

    class FastLedStripComponent: public Component {
      protected:
        CRGB *buffer = NULL;
        uint8_t hue = 0;
        bool firstPass = true;
        void init_fastled_buffer(uint16_t size);
        void copy_fastled_buffer(AddressableLight &it);
      public:
        void setup() override {}
        void loop() override {}
        void rainbow(AddressableLight &it, bool initial_run);
        void confetti(AddressableLight &it, bool initial_run);
        void sinelon(AddressableLight &it, bool initial_run);
        void beats(AddressableLight &it, bool initial_run);
        void juggle(AddressableLight &it, bool initial_run);
        void noise(AddressableLight &it, bool initial_run);
        void demo(AddressableLight &it, bool initial_run);
    };
  }
}
