#ifndef MICRAS_PROXY_ARGB_CPP
#define MICRAS_PROXY_ARGB_CPP

#include "micras/proxy/argb.hpp"
#include "physics/argb.hpp"
#include "physics/box2d_micrasbody.hpp"
#include <iostream>

namespace micras::proxy {
template <uint8_t num_of_leds>
TArgb<num_of_leds>::TArgb(const Config& config) : 
    micrasBody(config.micrasBody), 
    uncertainty(config.uncertainty), 
    brightness(config.brightness) {
    // LEDs will be initialized by Box2DMicrasBody
}

template <uint8_t num_of_leds>
TArgb<num_of_leds>::~TArgb() {
    // LEDs are automatically cleaned up when the vector is destroyed
}

template <uint8_t num_of_leds>
void TArgb<num_of_leds>::turn_on() {
    for (auto& argb : this->argbs) {
        argb.turnOn();
    }
}

template <uint8_t num_of_leds>
void TArgb<num_of_leds>::turn_off() {
    for (auto& argb : this->argbs) {
        argb.turnOff();
    }
}

template <uint8_t num_of_leds>
void TArgb<num_of_leds>::update() {
    // Update each ARGB component to recalculate its world position
    for (auto& argb : this->argbs) {
        argb.update();
    }
}

template <uint8_t num_of_leds>
void TArgb<num_of_leds>::attachArgb(b2Vec2 localPosition, b2Vec2 size, micrasverse::types::Color color) {
    this->argbs.push_back(micrasverse::physics::Argb(this->bodyId, localPosition, size, color));
}

template <uint8_t num_of_leds>
void TArgb<num_of_leds>::set_color(const micrasverse::types::Color& color, uint8_t index) {
    if (index < argbs.size()) {
        this->argbs.at(index).setColor({color.r, color.g, color.b});
    } else {
        std::cerr << "Warning: Attempted to set color for ARGB LED " << static_cast<int>(index) 
                  << " but only " << argbs.size() << " LEDs are available" << std::endl;
    }
}

template <uint8_t num_of_leds>
void TArgb<num_of_leds>::set_color(const micrasverse::types::Color& color) {
    for (auto& argb : this->argbs) {
        argb.setColor({color.r, color.g, color.b});
    }
}

template <uint8_t num_of_leds>
void TArgb<num_of_leds>::set_colors(const std::array<micrasverse::types::Color, num_of_leds>& colors) {
    for (uint8_t i = 0; i < std::min(static_cast<size_t>(num_of_leds), argbs.size()); i++) {
        this->set_color(colors.at(i), i);
    }
}

template <uint8_t num_of_leds>
void TArgb<num_of_leds>::encode_color(const micrasverse::types::Color& color, uint8_t index) {
    // Implementation for encoding color
    if (index < argbs.size()) {
        this->argbs.at(index).setColor({color.r, color.g, color.b});
    }
}

}  // namespace micras::proxy

#endif  // MICRAS_PROXY_ARGB_CPP
