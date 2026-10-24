#pragma once

#include "effect.h"

namespace esphome {
  namespace Nezumikun {

    class EffectFill : public Effect {
      protected:
        uint8_t x_ = 0;
        uint8_t y_ = 0;
        uint8_t hue_ = 0;
        uint8_t hue_step_ = 1;
      public:
        EffectFill(Canvas * canvas);
        ~EffectFill();
        virtual void reset() override;
        virtual void loop() override;    
    };

  }
}
