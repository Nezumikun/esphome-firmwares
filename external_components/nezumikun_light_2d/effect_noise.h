#pragma once

#include "effect.h"

namespace esphome {
  namespace Nezumikun {

    class EffectPerlinNoise : public Effect {
      protected:
        uint16_t x;
        uint16_t y;
        uint16_t z;
        uint8_t hue;

        uint16_t speed = 3; // a nice starting speed, mixes well with a scale of 100
        // uint16_t speed = 33;
        // uint16_t speed = 100; // wicked fast!

        // uint16_t scale = 1; // mostly just solid colors
        // uint16_t scale = 4011; // very zoomed out and shimmery
        uint16_t scale = 100;
        uint8_t * data;
        uint32_t start_time_ = 0;
        uint8_t submode_ = 0;
        void loop_perlin();
        void loop_paletted();
      public:
        EffectPerlinNoise(Canvas * canvas);
        ~EffectPerlinNoise();
        virtual void reset() override;
        virtual void loop() override;    
        virtual bool wait() override;
    };

  }
}
