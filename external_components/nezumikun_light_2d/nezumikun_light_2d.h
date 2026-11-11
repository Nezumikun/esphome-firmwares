#pragma once

#include "esphome/core/component.h"
#include "esphome/components/light/addressable_light.h"
#include "canvas.h"
#include "effects.h"

namespace esphome {
namespace nezumikun_light_2d {

class NezumikunLight2DComponent : public Component {
  public:
    NezumikunLight2DComponent();
    // Indicates if the UDP effect is active
    bool is_effect_active = false;
    
    void setup() override;
    void loop() override;
    void dump_config() override;
    ~NezumikunLight2DComponent();

    void set_strip(light::AddressableLightState* light_strip) { this->light_strip_ = light_strip; }
    void set_canvas_width(int width) { this->canvas_width_ = width; }
    void set_canvas_height(int height) { this->canvas_height_ = height; }
    void set_effect_active(bool active) { this->is_effect_active = active; }
    void set_mode_demo() { this->set_mode_(Mode::DEMO); }
    void set_mode_fill() { this->set_mode_(Mode::FILL); }
    void set_mode_rainbow() { this->set_mode_(Mode::RAINBOW); }
    void set_mode_noise() { this->set_mode_(Mode::NOISE); }

  protected:
    enum Mode {
      DEMO,
      FILL,
      RAINBOW,
      NOISE
    } mode_ = Mode::DEMO;
    bool is_init = false;
    // Internal members
    light::AddressableLightState* light_strip_{nullptr};
    Nezumikun::EffectInit* effect_init_{nullptr};
    Nezumikun::EffectFill* effect_fill_{nullptr};
    Nezumikun::EffectRainbow* effect_rainbow_{nullptr};
    Nezumikun::EffectPerlinNoise* effect_noise_{nullptr};

    // Returns the AddressableLight pointer or nullptr if not available
    light::AddressableLight* get_addressable_light_();
    Nezumikun::Canvas* get_canvas_();
    void show_(light::AddressableLight* addressable_light);
    void set_mode_(Mode mode);

    uint8_t hue_ = 0;
    uint8_t canvas_width_ = 0;
    uint8_t canvas_height_ = 0;
    uint8_t repeat_effect_ = 0;
    Nezumikun::Effect * current_effect_{nullptr};
    Nezumikun::Canvas * canvas_{nullptr};
};


}  // namespace esp32_udp_light
}  // namespace esphome