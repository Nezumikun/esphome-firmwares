#include "canvas.h"
#include "math.h"

namespace esphome {
  namespace Nezumikun {

    Canvas::Canvas(uint8_t width, uint8_t height) {
      this->width = width;
      this->height = height;
      this->ledsNumber = (uint16_t)width * height;
      this->leds = (RGB *) malloc((width * height) * sizeof(RGB));
      this->hsv = (HSV *) this->leds;
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

    void Canvas::fillHSV(const HSV color) {
      RGB rgb = { .red = 0, .green = 0, .blue = 0 };
      if (this->is_hsv) {
        rgb.red = color.hue;
        rgb.green = color.saturation;
        rgb.blue = color.value;
      }
      fill_solid(this->leds, this->ledsNumber, rgb);
    }

    void Canvas::fadeHSV(u_int8_t delta) {
      for (uint16_t i = 0; i < this->ledsNumber; i++) {
        if (this->hsv[i].value < delta) {
          this->hsv[i].value = 0;
        } else {
          this->hsv[i].value -= delta;
        }
      }
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

    void Canvas::setHSVFlag(bool is_hsv) {
      this->is_hsv = is_hsv;
    }

    bool Canvas::getHSVFlag() {
      return this->is_hsv;
    }

    void Canvas::setRotateRandom() {
      this->anngle = (Rotate) (random8(255) & 0x03);
    }

    uint16_t Canvas::getLedIndex(uint8_t x, uint8_t y, bool useRotateSettings) {
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
      return led_index;
    }

    void Canvas::setPixel(uint8_t x, uint8_t y, const RGB color, bool useRotateSettings) {
      uint16_t led_index = this->getLedIndex(x, y, useRotateSettings);
      this->is_hsv = false;
      // ESP_LOGI("TAG", "X = %i, Y = %i: %i", x, y, led_index);
      this->leds[led_index] = color;
    }

    void Canvas::setPixelHSV(uint8_t x, uint8_t y, const HSV color, bool useRotateSettings) {
      uint16_t led_index = this->getLedIndex(x, y, useRotateSettings);
      if (this->is_hsv) {
        this->hsv[led_index] = color;
      } else {
        RGB rgb = hsv_to_rgb(color);
        this->leds[led_index] = rgb;
      }
    }

    Canvas::Rotate Canvas::getRotate() {
      return this->anngle;
    }

  }
}
