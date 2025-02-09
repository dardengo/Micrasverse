#ifndef MICRAS_PROXY_ARGB_CPP
#define MICRAS_PROXY_ARGB_CPP

#include "proxy/argb.hpp"

#include <memory>

namespace micrasverse::proxy {
template <uint8_t num_of_leds>
TArgb<num_of_leds>::TArgb(const Config& config, b2BodyId bodyId) :
    brightness{config.max_brightness / 100.0F},
    bodyId{bodyId} {

    this->argbs.reserve(num_of_leds);
    this->attachArgb({0.0f, 0.0f}, {0.02f, 0.02f}, {static_cast<uint8_t>(235 * this->brightness), static_cast<uint8_t>(100 * this->brightness), static_cast<uint8_t>(25 * this->brightness)});
    
    for (auto& argb : this->argbs) {
        argb.turnOff();
    }
}

template <uint8_t num_of_leds>
void TArgb<num_of_leds>::attachArgb(b2Vec2 localPosition, b2Vec2 size, core::Color color) {
    this->argbs.push_back(physics::Argb(this->bodyId, localPosition, size, color));
}

template <uint8_t num_of_leds>
void TArgb<num_of_leds>::set_color(const core::Color& color, uint8_t index) {
    this->argbs.at(index).setColor({color.r, color.g, color.b});
}

template <uint8_t num_of_leds>
void TArgb<num_of_leds>::set_color(const core::Color& color) {
    for (auto& argb : this->argbs) {
        argb.setColor({color.r, color.g, color.b});
    }
}

template <uint8_t num_of_leds>
void TArgb<num_of_leds>::set_colors(const std::array<core::Color, num_of_leds>& colors) {
    for (uint8_t i = 0; i < num_of_leds; i++) {
        this->set_color(colors.at(i), i);
    }

}

template <uint8_t num_of_leds>
void TArgb<num_of_leds>::turn_off(uint8_t index) {
    this->argbs.at(index).turnOff();
}

template <uint8_t num_of_leds>
void TArgb<num_of_leds>::turn_off() {
    for (auto& argb : this->argbs) {
        argb.turnOff();
    }
}

template <uint8_t num_of_leds>
void TArgb<num_of_leds>::encode_color(const core::Color& color, uint8_t index) {

}

}  // namespace micrasverse::proxy

#endif  // MICRAS_PROXY_ARGB_CPP
