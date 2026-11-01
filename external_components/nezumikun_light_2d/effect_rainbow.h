#pragma once

#include "effect.h"

namespace esphome {
  namespace Nezumikun {

    class EffectRainbow : public Effect {
      protected:
        uint8_t hue_ = 0;
        uint8_t hue_step_ = 1;
      public:
        EffectRainbow(Canvas * canvas);
        ~EffectRainbow();
        virtual void reset() override;
        virtual void loop() override;    
        virtual bool wait() override;
    };

  }
}
