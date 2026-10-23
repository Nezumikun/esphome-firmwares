#include "esphome/core/log.h"
#include "nezumikun_light_2d.h"
#include "color.h"
#include "math.h"
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

    static uint8_t skip = 4;
    if (--skip == 0) {
        skip = 4;
    } else {
        return;
    }

    Nezumikun::HSV hsv = { .hue = this->hue_, .saturation = 0xff, .value = 0xff };
    Nezumikun::RGB rgb = Nezumikun::hsv_to_rgb(hsv);
    
    static uint8_t hue_step = 1;
    static uint8_t x = 0;
    static u_int8_t y = this->canvas_->getHeight();
    if (x >= this->canvas_->getWidth()) {
        y++;
        x = 0;
    }
    if (y >= this->canvas_->getHeight()) {
        Nezumikun::RGB black = { .red = 0, .green = 0, .blue = 0 };
        this->canvas_->fill(black);
        this->canvas_->setRotateRandom();
        hue_step = Nezumikun::random8(10);
        Nezumikun::Canvas::Rotate rotate = this->canvas_->getRotate();
        ESP_LOGI(
            TAG,
            "Rotate %i (W X H = %i x %i)", 
            rotate == Nezumikun::Canvas::Rotate::Angle270 ? 270 : rotate == Nezumikun::Canvas::Rotate::Angle180 ? 180 : rotate == Nezumikun::Canvas::Rotate::Angle90 ? 90 : 0,
            this->canvas_->getWidth(),
            this->canvas_->getHeight()
        );
        y = 0;
    } else {
        // ESP_LOGI(TAG, "X = %i, Y = %i: R=%x G=%x B=%x", x, y, rgb.red, rgb.green, rgb.blue);
        this->canvas_->setPixel(x, y, rgb);
        x++;
    }

    this->show_(addressable_light);

    this->hue_ += hue_step;
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
    for (uint16_t led_index = 0; led_index < this->canvas_->getLedsNumber(); led_index++) {
        uint8_t r = canvas_leds[led_index].red;
        uint8_t g = canvas_leds[led_index].green;
        uint8_t b = canvas_leds[led_index].blue;
        auto pixel = addressable_light->get(led_index);
        pixel.set(Color(r, g, b));
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