#include "effect_rainbow.h"
#include "math.h"

namespace esphome {
  namespace Nezumikun {

    EffectRainbow::EffectRainbow(Canvas * canvas) : Effect (canvas) {
      this->hue_ = 0;
    };

    EffectRainbow::~EffectRainbow() {
    };

    void EffectRainbow::reset() {
      this->is_finished_ = false;
      this->canvas_->setRotateRandom();
      this->canvas_->setMirrorFlag((random8() & 1) == 1);
      this->canvas_->setHSVFlag(true);
      HSV black = { .hue = 0, .saturation = 0, .value = 0 };
      this->canvas_->fillHSV(black);
    }

    void EffectRainbow::loop() {
      if (this->is_finished_) {
        this->reset();
      }

      HSV hsv = { .hue = this->hue_, .saturation = 0xff, .value = 0xff };
      this->canvas_->fillHSV(hsv);
      this->hue_++;
      if (this->hue_ == 0) {
        this->is_finished_ = true;
      }
    }
  
    bool EffectRainbow::wait() {
      static u_int8_t skip = 2;
      if (--skip == 0) {
          skip = 2;
          return false;
      } else {
          return true;
      }      
    }
  }
}
