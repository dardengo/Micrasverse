#include "micras/proxy/led.hpp"
#include "box2d/box2d.h"

namespace micras::proxy {

Led::Led(const Config& config, const b2BodyId bodyId) :
    bodyId{bodyId},
    state{config.initial_state},
    red{config.red},
    green{config.green},
    blue{config.blue} { }

void Led::turn_on() {
    this->state = true;
}

void Led::turn_off() {
    this->state = false;
}

void Led::toggle() {
    this->state = !this->state;
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

}  // namespace micras::proxy 
