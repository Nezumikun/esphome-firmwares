#include "effect.h"

namespace esphome {
  namespace Nezumikun {

    Effect::Effect(Canvas * canvas) {
      this->canvas_ = canvas;
    };

    Effect::~Effect() {
    };

    void Effect::reset() {
      this->is_finished_ = false;
    }

    bool Effect::isFinished() {
      return this->is_finished_;
    };

    void Effect::effectFinished() {
      this->is_finished_ = true;
    };

    void Effect::loop() {
    }

    bool Effect::wait() {
      return false;
    }

  }
}
