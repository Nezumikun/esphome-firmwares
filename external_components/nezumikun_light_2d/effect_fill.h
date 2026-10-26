#pragma once

#include "effect.h"

namespace esphome {
  namespace Nezumikun {

    class EffectFill : public Effect {
      protected:
        enum ColorStyle {
          SOLID,
          HUE_SHIFT_EACH_DOT,
          HUE_SHIFT_NEW_LINE,
          RANDOM_EACH_DOT,
          RANDOM_NEW_LINE
        } color_style_;
        uint8_t x_ = 0;
        uint8_t y_ = 0;
        uint8_t hue_ = 0;
        uint8_t hue_step_ = 1;
        bool continue_line_ = false;
        bool fade = false;
        u_int8_t fade_step = 25;
        uint8_t width_ = 0;
        uint8_t height_ = 0;
        uint16_t dot_count_ = 0;
      public:
        EffectFill(Canvas * canvas);
        ~EffectFill();
        virtual void reset() override;
        virtual void loop() override;    
    };

  }
}
