#include "effect_fire.h"
#include "math.h"

namespace esphome {
  namespace Nezumikun {

    EffectFire::EffectFire(Canvas * canvas) : Effect (canvas) {
    };

    void EffectFire::reset() {
    }

    void EffectFire::loop() {
      uint8_t width = this->canvas_->getWidth();
      uint8_t height = this->canvas_->getHeight();
      for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
          this->canvas_->setPixel(
            x,
            y,
            color_from_palette (
              gradientPaletteFire,
              qsub8 (inoise8 (x * 60 , y * 60+ a , a /3), 
              std::abs(y - (height-1)) * 255 / (height-1))
            )
          );    
        }                                                              
      }      
      this->a += 5;
    }

    bool EffectFire::wait() {
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