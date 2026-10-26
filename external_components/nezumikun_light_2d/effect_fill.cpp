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
      this->canvas_->setHSVFlag(true);
      this->continue_line_ = (random8() & 1) == 1;
      this->fade = (random8() & 1) == 1;
      this->fade_step = 10 + random8(15);
      this->x_ = 0;
      this->y_ = 0;
      this->width_ = this->canvas_->getWidth();
      this->height_ = this->canvas_->getHeight();
      this->dot_count_ = 0;
      HSV black = { .hue = 0, .saturation = 0, .value = 0 };
      this->canvas_->fillHSV(black);
      u_int8_t rnd = random8(5);
      this->color_style_ = (rnd == 0) ? ColorStyle::SOLID
        : (rnd == 1) ? ColorStyle::HUE_SHIFT_EACH_DOT
        : (rnd == 2) ? ColorStyle::HUE_SHIFT_NEW_LINE
        : (rnd == 3) ? ColorStyle::RANDOM_EACH_DOT
        : ColorStyle::RANDOM_NEW_LINE
      ;
      this->hue_ = random8();
      this->hue_step_ = random8(15) + (this->color_style_ == ColorStyle::HUE_SHIFT_NEW_LINE ? 30 : 0);
    }

    void EffectFill::loop() {
      if (this->is_finished_) {
        this->reset();
      }
      
      if (this->x_ >= this->width_) {
        this->y_++;
        this->x_ = (this->continue_line_ && ((this->y_ & 1) == 1)) ? (this->width_ - 1) : 0;
        if (this->color_style_ == ColorStyle::HUE_SHIFT_NEW_LINE) {
          this->hue_ += this->hue_step_;
        }
        else if (this->color_style_ == ColorStyle::RANDOM_NEW_LINE) {
          this->hue_ += random8();
        }
      }
      if (this->y_ >= this->height_) {
        this->y_ = 0;
      } else {
        if (this->fade) {
          this->canvas_->fadeHSV(this->fade_step);
        }
        HSV hsv = { .hue = this->hue_, .saturation = 0xff, .value = 0xff };
        this->canvas_->setPixelHSV(this->x_, this->y_, hsv);
        this->dot_count_++;
        this->x_ += (this->continue_line_ && ((this->y_ & 1) == 1)) ? -1 : 1;
      }

      if (this->dot_count_ == ((uint16_t)this->width_ * this->height_)) {
        this->is_finished_ = true;
      }

      if (this->color_style_ == ColorStyle::HUE_SHIFT_EACH_DOT) {
        this->hue_ += this->hue_step_;
      }
      else if (this->color_style_ == ColorStyle::RANDOM_EACH_DOT) {
        this->hue_ += random8();
      }
    }

  }
}
