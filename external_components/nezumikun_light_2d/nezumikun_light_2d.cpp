#include "esphome/core/log.h"
#include "nezumikun_light_2d.h"
#include "color.h"
#include "esphome/components/wifi/wifi_component.h"

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
    auto canvas = this->get_canvas_();
    if (canvas == nullptr) return;

    static uint8_t skip = 1;
    if (--skip == 0) {
        skip = 1;
    } else {
        return;
    }

    // Update LEDs with received data
    int led_count = addressable_light->size();

    // Log RGB values
    Nezumikun::HSV hsv = { .hue = this->hue_, .saturation = 0xff, .value = 0xff };
    Nezumikun::RGB rgb = Nezumikun::hsv_to_rgb(hsv);
    this->canvas_->fill(rgb);
    this->show_(addressable_light);
    // for (int led_index = 0; led_index < led_count; led_index++) {
    //     hsv.hue += 10;
    //     Nezumikun::RGB rgb = Nezumikun::hsv_to_rgb(hsv);
    //     uint8_t r = rgb.red;
    //     uint8_t g = rgb.green;
    //     uint8_t b = rgb.blue;
    // // Uncomment the following line only for debugging
    // // ESP_LOGI(TAG, "LED %d: R=%d G=%d B=%d", led_index, r, g, b);
    //     auto pixel = addressable_light->get(led_index);
    //     pixel.set(Color(r, g, b));
    // }
    this->hue_++;
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
    Nezumikun::RGB * canvas_leds = this->canvas_->getLeds();
    uint8_t width = this->canvas_->getWidth(false);
    for (uint8_t y = 0; y < this->canvas_->getHeight(false); y++) {
        for (uint8_t x = 0; x < width; x++) {
            // Serial.println(String("x = ") + x + " y = " + y);
            uint16_t led_index = this->canvas_->XY(x, y);
            uint16_t canvas_index = (uint16_t)y * width + x;
            uint8_t r = canvas_leds[canvas_index].red;
            uint8_t g = canvas_leds[canvas_index].green;
            uint8_t b = canvas_leds[canvas_index].blue;
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