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
      protected:
        uint8_t width;
        uint8_t height;
        uint16_t ledsNumber;
        RGB * leds;
        HSV * hsv;
        uint8_t brightness = 100;
        Rotate anngle = Rotate::None;
        bool is_mirror = false;
        bool is_hsv = false;
        uint16_t getLedIndex(uint8_t x, uint8_t y, bool useRotateSettings = true);
      public:
        Canvas(uint8_t width, uint8_t height);
        uint8_t getWidth(bool useRotateSettings = true);
        uint8_t getHeight(bool useRotateSettings = true);
        uint8_t getBrightness();
        uint16_t getLedsNumber();
        void setBrightness(uint8_t brightness);
        RGB * getLeds();
        void fill(const RGB color);
        void fillHSV(const HSV color);
        void setPixel(uint8_t x, uint8_t y, const RGB color, bool useRotateSettings = true);
        void setPixelHSV(uint8_t x, uint8_t y, const HSV color, bool useRotateSettings = true);
        void fadeHSV(u_int8_t delta);
        void setRotate(Rotate angle);
        Rotate getRotate();
        void setRotateRandom();
        void setMirrorFlag(bool is_mirror);
        bool getMirrorFlag();
        void setHSVFlag(bool is_hsv);
        bool getHSVFlag();
    };

  }
}
