#ifndef MICRAS_PROXY_ARGB_CPP
#define MICRAS_PROXY_ARGB_CPP

#include "micras/proxy/argb.hpp"
#include "physics/argb.hpp"

namespace micras::proxy {
template <uint8_t num_of_leds>
TArgb<num_of_leds>::TArgb(const Config& config, const b2BodyId bodyId) : bodyId(bodyId), uncertainty(config.uncertainty), brightness(config.brightness) {

    // Use the first brightness value for the initial ARGB
    float brightnessValue = this->brightness[0];
    this->attachArgb({0.0f, 0.0f}, {0.02f, 0.02f}, {static_cast<uint8_t>(235 * brightnessValue), static_cast<uint8_t>(100 * brightnessValue), static_cast<uint8_t>(25 * brightnessValue)});
    
    for (auto& argb : this->argbs) {
        argb.turnOff();
    }
}

template <uint8_t num_of_leds>
TArgb<num_of_leds>::~TArgb() {}

template <uint8_t num_of_leds>
void TArgb<num_of_leds>::turn_on() {}

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
    this->argbs.at(index).setColor({color.r, color.g, color.b});
}

template <uint8_t num_of_leds>
void TArgb<num_of_leds>::set_color(const micrasverse::types::Color& color) {
    for (auto& argb : this->argbs) {
        argb.setColor({color.r, color.g, color.b});
    }
}

template <uint8_t num_of_leds>
void TArgb<num_of_leds>::set_colors(const std::array<micrasverse::types::Color, num_of_leds>& colors) {
    for (uint8_t i = 0; i < num_of_leds; i++) {
        this->set_color(colors.at(i), i);
    }
}

template <uint8_t num_of_leds>
void TArgb<num_of_leds>::encode_color(const micrasverse::types::Color& color, uint8_t index) {

}

}  // namespace micras::proxy

#endif  // MICRAS_PROXY_ARGB_CPP
