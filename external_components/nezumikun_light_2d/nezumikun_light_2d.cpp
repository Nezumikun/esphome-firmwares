#include "esphome/core/log.h"
#include "nezumikun_light_2d.h"
#include "color.h"
#include "math.h"
#include "effect_fill.h"

namespace esphome {
namespace nezumikun_light_2d {


// Constructor: initializes members
NezumikunLight2DComponent::NezumikunLight2DComponent()
    : is_effect_active(false), light_strip_(nullptr) {}

static const char *TAG = "nezumikun_light_2d.component";

// Initial component setup
void NezumikunLight2DComponent::setup() {
    ESP_LOGI(TAG, "NezumikunLight2DComponent setup() called");
}


// Main loop: handles UDP reception and LED update
void NezumikunLight2DComponent::loop() {
    // Get the AddressableLight object
    auto addressable_light = this->get_addressable_light_();
    if (addressable_light == nullptr) return;

    // Check if the strip is ON 
    if (!this->light_strip_->current_values.is_on()) return;    

    // Check if the current effect is one of the allowed UDP effect names
    std::string current_effect = this->light_strip_->get_effect_name();
    if (current_effect != "Demo" &&
        current_effect != "UDP Effect") {
        this->is_effect_active = false;
        return;
    }

    auto canvas = this->get_canvas_();
    if (canvas == nullptr) return;

    if (!this->current_effect_) {
        this->current_effect_ = (!this->is_init) ? ((Nezumikun::Effect *)new Nezumikun::EffectInit(canvas)) : ((Nezumikun::Effect *)new Nezumikun::EffectFill(canvas));
    }

    if (this->current_effect_->wait()) {
        return;
    }

    this->current_effect_->loop();
    if (!this->is_init && this->current_effect_->isFinished()) {
        this->is_init = true;
        delete this->current_effect_;
        this->current_effect_ = nullptr;
    }

    this->show_(addressable_light);
    addressable_light->schedule_show();
}


// Show the component configuration in the log
void NezumikunLight2DComponent::dump_config() {
    ESP_LOGCONFIG(TAG, "NezumikunLight2DComponent: ");
}


// Destructor: closes the socket if it is open
NezumikunLight2DComponent::~NezumikunLight2DComponent() {
}

// Returns the AddressableLight pointer or nullptr if not available
light::AddressableLight* NezumikunLight2DComponent::get_addressable_light_() {
    if (!this->light_strip_) return nullptr;
    auto light_output = this->light_strip_->get_output();
    if (light_output == nullptr) return nullptr;
    return static_cast<light::AddressableLight*>(light_output);
}

Nezumikun::Canvas* NezumikunLight2DComponent::get_canvas_() {
    if (!this->canvas_) {
        if ((this->canvas_width_ == 0) || (this->canvas_height_ == 0))
            return nullptr;
        this->canvas_ = new Nezumikun::Canvas(this->canvas_width_, this->canvas_height_);
    }
    return this->canvas_;
}

void NezumikunLight2DComponent::show_(light::AddressableLight* addressable_light) {
    if (this->canvas_->getHSVFlag()) {
        Nezumikun::HSV * canvas_leds = (Nezumikun::HSV *) this->canvas_->getLeds();
        for (uint16_t led_index = 0; led_index < this->canvas_->getLedsNumber(); led_index++) {
            Nezumikun::RGB rgb = hsv_to_rgb(canvas_leds[led_index]);
            uint8_t r = rgb.red;
            uint8_t g = rgb.green;
            uint8_t b = rgb.blue;
            auto pixel = addressable_light->get(led_index);
            pixel.set(Color(r, g, b));
        }
    } else {
        Nezumikun::RGB * canvas_leds = this->canvas_->getLeds();
        for (uint16_t led_index = 0; led_index < this->canvas_->getLedsNumber(); led_index++) {
            uint8_t r = canvas_leds[led_index].red;
            uint8_t g = canvas_leds[led_index].green;
            uint8_t b = canvas_leds[led_index].blue;
            auto pixel = addressable_light->get(led_index);
            pixel.set(Color(r, g, b));
        }
    }
    // for (uint8_t i = 0; i < this->skipCount; i++) {
    // for (uint16_t p = this->skip[i].start; p < this->skip[i].start + this->skip[i].length; p++) {
    //     this->leds[p] = CRGB::Black;
    // }
    // }
    // FastLED.show(this->canvas->getBrightness());
}

}  // namespace nezumikun_light_2d
}  // namespace esphome