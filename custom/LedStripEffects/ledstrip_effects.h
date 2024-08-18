#pragma once

#include "esphome/core/component.h"
#include "esphome/core/defines.h"
#include "esphome/core/color.h"
#include "esphome/core/log.h"
#include "esphome/components/light/addressable_light.h"
#include "color.h"

namespace esphome {
  namespace Nezumikun {
    class LedStripEffectsComponent: public Component {
      protected:
        color * buffer = NULL;
        uint8_t hue = 0;
        bool testComplited = false;
        bool firstPass = true;
        void init_buffer(uint16_t size);
        void copy_buffer_rgb(esphome::light::AddressableLight &it);
        void copy_buffer_hsv(esphome::light::AddressableLight &it);
        void fill_solid_rgb(uint8_t r, uint8_t g, uint8_t b, uint16_t size);
        void fade_to_black_hsv(uint8_t step, uint16_t size);
        void fade_to_black_rgb(uint8_t percent, uint16_t size);
        void test(esphome::light::AddressableLight &it, bool initial_run);
      public:
        void setup() override {}
        void loop() override {}
        void rainbow(esphome::light::AddressableLight &it, bool initial_run);
        void confetti(esphome::light::AddressableLight &it, bool initial_run);
        void sinelon(esphome::light::AddressableLight &it, bool initial_run);
        void beats(esphome::light::AddressableLight &it, bool initial_run);
        void juggle(esphome::light::AddressableLight &it, bool initial_run);
        void noise(esphome::light::AddressableLight &it, bool initial_run);
        void demo(esphome::light::AddressableLight &it, bool initial_run);
    };
  }
}
