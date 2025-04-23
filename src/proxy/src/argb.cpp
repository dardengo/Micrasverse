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
    // Connect to physical ARGBs from Box2DMicrasBody
    if (micrasBody) {
        auto& physicalArgbs = micrasBody->getArgbs();
        for (size_t i = 0; i < physicalArgbs.size(); ++i) {
            if (physicalArgbs[i]) {
                argbRefs.emplace_back(std::ref(*physicalArgbs[i]));
            }
        }
        
        if (argbRefs.empty()) {
            std::cerr << "Warning: No physical ARGBs were found in the MicrasBody" << std::endl;
        }
    } else {
        std::cerr << "Warning: No MicrasBody provided, ARGB proxy will not be functional" << std::endl;
    }
}

template <uint8_t num_of_leds>
TArgb<num_of_leds>::~TArgb() {
    // No cleanup needed for references
}

template <uint8_t num_of_leds>
void TArgb<num_of_leds>::turn_on() {
    for (auto& argbRef : this->argbRefs) {
        argbRef.get().turnOn();
    }
}

template <uint8_t num_of_leds>
void TArgb<num_of_leds>::turn_off() {
    for (auto& argbRef : this->argbRefs) {
        argbRef.get().turnOff();
    }
}

template <uint8_t num_of_leds>
void TArgb<num_of_leds>::update() {
    // Physical ARGBs are updated by the physics system
    // We don't need to update them here
}

template <uint8_t num_of_leds>
void TArgb<num_of_leds>::attachArgb(b2Vec2 localPosition, b2Vec2 size, micrasverse::types::Color color) {
    // This method is no longer needed since we use physical ARGBs
    // However, we keep it for API compatibility
    std::cerr << "Warning: attachArgb called on proxy, but it has no effect. ARGBs are managed by Box2DMicrasBody" << std::endl;
}

template <uint8_t num_of_leds>
void TArgb<num_of_leds>::set_color(const micrasverse::types::Color& color, uint8_t index) {
    if (index < argbRefs.size()) {
        argbRefs[index].get().turnOn(color);
    } else {
        std::cerr << "Warning: Attempted to set color for ARGB LED " << static_cast<int>(index) 
                  << " but only " << argbRefs.size() << " LEDs are available" << std::endl;
    }
}

template <uint8_t num_of_leds>
void TArgb<num_of_leds>::set_color(const micrasverse::types::Color& color) {
    for (auto& argbRef : this->argbRefs) {
        argbRef.get().turnOn(color);
    }
}

template <uint8_t num_of_leds>
void TArgb<num_of_leds>::set_colors(const std::array<micrasverse::types::Color, num_of_leds>& colors) {
    for (uint8_t i = 0; i < std::min(static_cast<size_t>(num_of_leds), argbRefs.size()); i++) {
        this->set_color(colors.at(i), i);
    }
}

template <uint8_t num_of_leds>
void TArgb<num_of_leds>::encode_color(const micrasverse::types::Color& color, uint8_t index) {
    if (index < argbRefs.size()) {
        argbRefs[index].get().encode_color(color, index);
        argbRefs[index].get().turnOn();
    }
}

}  // namespace micras::proxy

#endif  // MICRAS_PROXY_ARGB_CPP
