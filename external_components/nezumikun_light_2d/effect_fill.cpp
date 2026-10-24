#include "effect_fill.h"
#include "math.h"

namespace esphome {
  namespace Nezumikun {

    EffectFill::EffectFill(Canvas * canvas) : Effect (canvas) {
    };

    EffectFill::~EffectFill() {
    };

    void EffectFill::reset() {
        this->is_finished_ = false;
        this->canvas_->setRotateRandom();
        this->canvas_->setMirrorFlag(random8() & 1 == 1);
        this->x_ = 0;
        this->y_ = 0;
        RGB black = { .red = 0, .green = 0, .blue = 0 };
        this->canvas_->fill(black);
        this->hue_step_ = random8(30);
    }

    void EffectFill::loop() {
      if (this->is_finished_) {
        this->reset();
      }
      HSV hsv = { .hue = this->hue_, .saturation = 0xff, .value = 0xff };
      RGB rgb = hsv_to_rgb(hsv);
      
      if (this->x_ >= this->canvas_->getWidth()) {
          this->y_++;
          this->x_ = 0;
      }
      if (this->y_ >= this->canvas_->getHeight()) {
          this->y_ = 0;
      } else {
          this->canvas_->setPixel(this->x_, this->y_, rgb);
          this->x_++;
      }

      if ((this->x_ == this->canvas_->getWidth()) && (this->y_ == this->canvas_->getHeight() - 1)) {
        this->is_finished_ = true;
      }

      this->hue_ += this->hue_step_;
    }

  }
}
