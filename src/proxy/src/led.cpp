#include "micras/proxy/led.hpp"
#include "physics/box2d_micrasbody.hpp"
#include "physics/box2d_led.hpp"
#include "micras/core/types.hpp"
#include "micrasverse_core/types.hpp"

namespace micras::proxy {

Led::Led(const Config& config) :
    micrasBody{config.micrasBody},
    state{config.initial_state},
    red{config.red},
    green{config.green},
    blue{config.blue} { }

void Led::turn_on() {
    this->state = true;
    updateColor();
}

void Led::turn_off() {
    this->state = false;
    updateColor();
}

void Led::toggle() {
    this->state = !this->state;
    updateColor();
}

bool Led::is_on() const {
    return this->state;
}

uint8_t Led::get_red() const {
    return this->red;
}

uint8_t Led::get_green() const {
    return this->green;
}

uint8_t Led::get_blue() const {
    return this->blue;
}

void Led::setState(bool state) {
    if (state) {
        turn_on();
    } else {
        turn_off();
    }
}

void Led::updateColor() {
    if (this->state) {
        micrasBody->getLed().setColor(micrasverse::types::Color{
            static_cast<float>(red) / 255.0f,
            static_cast<float>(green) / 255.0f,
            static_cast<float>(blue) / 255.0f
        });
    } else {
        micrasBody->getLed().setColor(micrasverse::types::Color{0.0f, 0.0f, 0.0f});
    }
}

}  // namespace micras::proxy 
