#pragma once

#include "canvas.h"

namespace esphome {
  namespace Nezumikun {

    class Effect {
      protected:
        Canvas * canvas_;
        bool is_finished_ = true;
        virtual void effectFinished();
      public:
        Effect(Canvas * canvas);
        virtual ~Effect();
        bool isFinished();
        virtual void reset();
        virtual void loop() = 0;
    };

  }
}
