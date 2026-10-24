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
        bool is_mirror = false;
      public:
        Canvas(uint8_t width, uint8_t height);
        uint8_t getWidth(bool useRotateSettings = true);
        uint8_t getHeight(bool useRotateSettings = true);
        uint8_t getBrightness();
        uint16_t getLedsNumber();
        void setBrightness(uint8_t brightness);
        RGB * getLeds();
        void fill(const RGB color);
        void setPixel(uint8_t x, uint8_t y, const RGB color, bool useRotateSettings = true);
        void setRotate(Rotate angle);
        Rotate getRotate();
        void setRotateRandom();
        void setMirrorFlag(bool is_mirror);
        bool getMirrorFlag();
    };

  }
}
