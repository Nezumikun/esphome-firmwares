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
        this->canvas_->setMirrorFlag((random8() & 1) == 1);
        this->continue_line_ = (random8() & 1) == 1;
        this->x_ = 0;
        this->y_ = 0;
        this->width_ = this->canvas_->getWidth();
        this->height_ = this->canvas_->getHeight();
        this->dot_count_ = 0;
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
      
      if (this->x_ >= this->width_) {
          this->y_++;
          this->x_ = (this->continue_line_ && ((this->y_ & 1) == 1)) ? (this->width_ - 1) : 0;
      }
      if (this->y_ >= this->height_) {
          this->y_ = 0;
      } else {
          this->canvas_->setPixel(this->x_, this->y_, rgb);
          this->dot_count_++;
          this->x_ += (this->continue_line_ && ((this->y_ & 1) == 1)) ? -1 : 1;
      }

      if (this->dot_count_ == ((uint16_t)this->width_ * this->height_)) {
        this->is_finished_ = true;
      }

      this->hue_ += this->hue_step_;
    }

  }
}
