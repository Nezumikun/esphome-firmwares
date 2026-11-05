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
      this->submode_ = random8(5);
    }

    void EffectPerlinNoise::loop_perlin() {
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
    }

    void EffectPerlinNoise::loop_paletted() {
      uint8_t width = this->canvas_->getWidth();
      uint8_t height = this->canvas_->getHeight();
      for (uint8_t y = 0; y < height; y++) {
        for (uint8_t x = 0; x < width; x++) {
          Nezumikun::RGB color = color_from_palette(
            this->submode_ == 1 ? Nezumikun::palettePartyColors :
            this->submode_ == 2 ? Nezumikun::paletteHeatColors :
            this->submode_ == 3 ? Nezumikun::paletteRainbowColors :
            this->submode_ == 4 ? Nezumikun::paletteLavaColors :
            Nezumikun::paletteCloudColors, inoise8(this->y + y * scale, this->x + x * scale, this->z)
          );
          this->canvas_->setPixel(x, y, color);
        }
      }
      this->z += this->speed;
      // this->x++;
      // this->y--;
    }

    void EffectPerlinNoise::loop() {
      if (this->is_finished_) {
        this->reset();
      }

      if (this->submode_ == 0) {
        this->loop_perlin();
      } else {
        this->loop_paletted();
      }

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