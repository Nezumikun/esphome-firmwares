#pragma once

#include "color.h"

namespace esphome {
  namespace Nezumikun {

    class Canvas {
      public:
        enum class Rotate {
          None,
          Angle90,
          Angle180,
          Angle270
        };
      private:
        uint8_t width;
        uint8_t height;
        uint16_t ledsNumber;
        RGB * leds;
        uint8_t brightness = 100;
        Rotate anngle = Rotate::None;
      public:
        Canvas(uint8_t width, uint8_t height);
        uint8_t getWidth(bool useRotateSettings = true);
        uint8_t getHeight(bool useRotateSettings = true);
        uint8_t getBrightness();
        uint16_t getLedsNumber();
        void setBrightness(uint8_t brightness);
        RGB * getLeds();
        uint8_t X(uint8_t x, uint8_t y, bool useRotateSettings = true);
        uint8_t Y(uint8_t x, uint8_t y, bool useRotateSettings = true);
        uint16_t XY(uint8_t x, uint8_t y, bool useRotateSettings = true);
        void fill(const RGB color);
        void setRotate(Rotate angle);
        Rotate getRotate();
        void setRotateRandom();
    };

  }
}
