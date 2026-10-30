#include "effect_init.h"
// #include "esphome/core/log.h"

namespace esphome {
  namespace Nezumikun {

    // static const char *TAG = "nezumikun_light_2d.component";

    EffectInit::EffectInit(Canvas * canvas) : Effect (canvas) {
      this->reset();
    };

    EffectInit::~EffectInit() {
    };

    void EffectInit::reset() {
      this->is_finished_ = false;
      this->canvas_->setHSVFlag(false);
      this->fade = false;
      this->mode_ = Mode::INIT;
    }

    void EffectInit::loop() {
      if (this->is_finished_) {
        return;
      }
      if ((this->brightness == 0xff) && !this->fade) {
        this->fade = true;
      } else if (this->fade && (this->brightness == 0x00)) {
        this->fade = false;
        this->mode_ = (this->mode_ == Mode::RED) ? Mode::GREEN : (this->mode_ == Mode::GREEN) ? Mode::BLUE : (this->mode_ == Mode::BLUE) ? Mode::WHITE : Mode::FINISH;
        this->is_finished_ = (this->mode_ == Mode::FINISH);
      }
      if (this->mode_ == Mode::INIT) {
        this->mode_ = Mode::RED;
      }
      // ESP_LOGI(TAG, "this->mode_ = %i, this->brightness = %i", this->mode_, this->brightness);

      if (this->mode_ == Mode::RED) {
        this->rgb.red = this->brightness;
        this->rgb.green = 0;
        this->rgb.blue = 0;
      } else if (this->mode_ == Mode::GREEN) {
        this->rgb.green = this->brightness;
        this->rgb.red = 0;
        this->rgb.blue = 0;
      } else if (this->mode_ == Mode::BLUE) {
        this->rgb.blue = this->brightness;
        this->rgb.red = 0;
        this->rgb.green = 0;
      } else {
        this->rgb.red = this->brightness;
        this->rgb.green = this->brightness;
        this->rgb.blue = this->brightness;
      }
      this->canvas_->fill(this->rgb);

      this->brightness += this->fade ? -5 : 5;
    }

  }
}
