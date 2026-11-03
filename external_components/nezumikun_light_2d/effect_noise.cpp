#include "effect_noise.h"
#include "math.h"

namespace esphome {
  namespace Nezumikun {

    EffectPerlinNoise::EffectPerlinNoise(Canvas * canvas) : Effect (canvas) {
      this->x = random16();
      this->y = random16();
      this->z = random16();
      this->hue = random8();
      this->data = (uint8_t *) malloc(this->canvas_->getWidth() * this->canvas_->getHeight() * sizeof(uint8_t));
      this->canvas_->setRotate(Canvas::Rotate::None);
    };

    EffectPerlinNoise::~EffectPerlinNoise() {
      free(this->data);
    }

    void EffectPerlinNoise::reset() {
      this->is_finished_ = false;
      this->start_time_ = esphome::millis();
    }

    void EffectPerlinNoise::loop() {
      if (this->is_finished_) {
        this->reset();
      }
      uint8_t width = this->canvas_->getWidth();
      for (uint8_t i = 0; i < this->canvas_->getHeight(false); i++) {
        uint16_t ioffset = this->scale * i;
        for (uint8_t j = 0; j < width; j++) {
          uint16_t joffset = this->scale * j;
          uint8_t temp = inoise8(this->x + ioffset, this->y + joffset, this->z);
          this->data[i * width + j] = temp;
        }
      }
      this->z += this->speed;
      HSV hsv = { .hue = 0, .saturation = 255, .value = 0};
      for (uint8_t i = 0; i < this->canvas_->getHeight(); i++) {
        for (uint8_t j = 0; j < width; j++) {
          hsv.hue = this->hue + (this->data[i * width + j]);
          hsv.value = this->data[j * width + i];
          this->canvas_->setPixelHSV(j, i, hsv);
        }
      }
      this->x++;
      this->y--;

      uint32_t now = esphome::millis();
      uint32_t elapsed = now - start_time_;

      this->is_finished_ = (elapsed >= 15 * 1000);
    }

    bool EffectPerlinNoise::wait() {
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