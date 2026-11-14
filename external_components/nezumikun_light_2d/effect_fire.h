#pragma once

#include "effect.h"

namespace esphome {
  namespace Nezumikun {

    class EffectFire : public Effect {
      protected:
        uint16_t a = 0;
        uint32_t start_time_ = 0;
      public:
        EffectFire(Canvas * canvas);
        virtual void reset() override;
        virtual void loop() override;    
        virtual bool wait() override;
    };

  }
}
