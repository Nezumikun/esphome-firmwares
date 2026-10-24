#include "canvas.h"
#include "math.h"

namespace esphome {
  namespace Nezumikun {

    Canvas::Canvas(uint8_t width, uint8_t height) {
      this->width = width;
      this->height = height;
      this->ledsNumber = (uint16_t)width * height;
      this->leds = (RGB *) malloc((width * height) * sizeof(RGB));
      this->anngle = Rotate::None;
    }

    uint8_t Canvas::getWidth(bool useRotateSettings) {
      return (!useRotateSettings || (this->anngle == Rotate::None) || (this->anngle == Rotate::Angle180)) ? this->width : this->height;
    };

    uint8_t Canvas::getHeight(bool useRotateSettings) {
      return (!useRotateSettings || (this->anngle == Rotate::None) || (this->anngle == Rotate::Angle180)) ? this->height : this->width;
    };

    uint16_t Canvas::getLedsNumber() {
      return this->ledsNumber;
    };

    uint8_t Canvas::getBrightness() {
      return this->brightness;
    };

    void Canvas::setBrightness(uint8_t brightness) {
      this->brightness = brightness;
    };

    RGB * Canvas::getLeds() {
      return this->leds;
    };

    void Canvas::fill(const RGB color) {
      fill_solid(this->leds, this->ledsNumber, color);
    }

    void Canvas::setRotate(Rotate angle) {
      this->anngle = angle;
    }

    void Canvas::setMirrorFlag(bool is_mirror) {
      this->is_mirror = is_mirror;
    }

    bool Canvas::getMirrorFlag() {
      return this->is_mirror;
    }

    void Canvas::setRotateRandom() {
      this->anngle = (Rotate) (random8(255) & 0x03);
    }

    void Canvas::setPixel(uint8_t x, uint8_t y, const RGB color, bool useRotateSettings) {
      uint16_t led_index = 0;
      uint8_t lx = x;
      uint8_t ly = y;
      if (useRotateSettings) {
        if (this->anngle == Rotate::Angle180) {
          lx = this->width - x - 1;
          ly = this->height - y - 1;
        }
        else if (this->anngle == Rotate::Angle90) {
          lx = y;
          ly = this->height - x - 1;
        }
        else if (this->anngle == Rotate::Angle270) {
          lx = this->width - y - 1;
          ly = x;
        }
      }
      if (this->is_mirror) {
        if ((this->anngle == Rotate::Angle180) || (this->anngle == Rotate::None)) {
          lx = this->width - lx - 1;
        }
        else  {
          ly = this->height - ly - 1;
        }
      }
      led_index = ly * this->width;
      if ((ly & 1) == 1) {
        led_index += width - lx - 1;
      }
      else {
        led_index += lx;
      }
      // ESP_LOGI("TAG", "X = %i, Y = %i: %i", x, y, led_index);
      this->leds[led_index] = color;
    }

    Canvas::Rotate Canvas::getRotate() {
      return this->anngle;
    }

  }
}