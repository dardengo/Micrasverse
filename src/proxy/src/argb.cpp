#ifndef MICRAS_PROXY_ARGB_CPP
#define MICRAS_PROXY_ARGB_CPP

#include "proxy/argb.hpp"

#include <memory>

namespace micrasverse::proxy {
template <uint8_t num_of_leds>
TArgb<num_of_leds>::TArgb(const Config& config, b2BodyId bodyId) :
    brightness{config.max_brightness / 100.0F},
    bodyId{bodyId} {

    this->argbs.reserve(1);
    this->attachArgb({0.0f, 0.0f}, {0.02f, 0.02f}, {231.0f, 112.0f, 35.0f});

    for (auto& argb : this->argbs) {
        argb.turnOff();
    }
}

template <uint8_t num_of_leds>
void TArgb<num_of_leds>::attachArgb(b2Vec2 localPosition, b2Vec2 size, glm::vec3 color) {
    physics::Argb argb = physics::Argb(this->bodyId, localPosition, size, color);
    this->argbs.push_back(argb);
}

template <uint8_t num_of_leds>
void TArgb<num_of_leds>::set_color(const Color& color, uint8_t index) {
    this->argbs.at(index).setColor({color.red, color.green, color.blue});
}

template <uint8_t num_of_leds>
void TArgb<num_of_leds>::set_color(const Color& color) {
    for (auto& argb : this->argbs) {
        argb.setColor({color.red, color.green, color.blue});
    }
}

template <uint8_t num_of_leds>
void TArgb<num_of_leds>::set_colors(const std::array<Color, num_of_leds>& colors) {
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
void TArgb<num_of_leds>::encode_color(const Color& color, uint8_t index) {

}

}  // namespace micrasverse::proxy

#endif  // MICRAS_PROXY_ARGB_CPP
