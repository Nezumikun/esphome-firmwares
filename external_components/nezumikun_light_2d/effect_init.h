#pragma once

#include "effect.h"
#include "color.h"

namespace esphome {
  namespace Nezumikun {

    class EffectInit : public Effect {
      protected:
        enum Mode {
          INIT,
          RED,
          GREEN,
          BLUE,
          WHITE,
          FINISH
        } mode_;
        RGB rgb = { .red = 0, .green = 0, .blue = 0 };
        u_int8_t brightness = 0;
        bool fade = false;
      public:
        EffectInit(Canvas * canvas);
        ~EffectInit();
        virtual void reset() override;
        virtual void loop() override;    
    };

  }
}
