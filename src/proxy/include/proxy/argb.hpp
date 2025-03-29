#ifndef MICRAS_PROXY_ARGB_HPP
#define MICRAS_PROXY_ARGB_HPP

#include "physics/argb.hpp"
#include "core/types.hpp"

#include "box2d/box2d.h"

#include <vector>
#include <array>
#include <cstdint>

namespace micras::proxy {

template <uint8_t num_of_leds>
class TArgb {
public:
    std::vector<micrasverse::physics::Argb> argbs;

    struct Config {
        float max_brightness;
    };

    struct Colors {
        Colors() = delete;

        static constexpr micrasverse::core::Color red{255, 0, 0};
        static constexpr micrasverse::core::Color green{0, 255, 0};
        static constexpr micrasverse::core::Color blue{0, 0, 255};
        static constexpr micrasverse::core::Color yellow{255, 255, 0};
        static constexpr micrasverse::core::Color cyan{0, 255, 255};
        static constexpr micrasverse::core::Color magenta{255, 0, 255};
        static constexpr micrasverse::core::Color white{255, 255, 255};
    };

    explicit TArgb(const Config& config, b2BodyId bodyId);

    void attachArgb(b2Vec2 localPosition, b2Vec2 size, micrasverse::core::Color color);

    void set_color(const micrasverse::core::Color& color, uint8_t index);

    void set_color(const micrasverse::core::Color& color);

    void set_colors(const std::array<micrasverse::core::Color, num_of_leds>& colors);

    void turn_off(uint8_t index);

    void turn_off();

private:
    float brightness;

    b2BodyId bodyId;

    void encode_color(const micrasverse::core::Color& color, uint8_t index);
};
}  // namespace micras::proxy

#include "../../src/argb.cpp"

#endif  // MICRAS_PROXY_ARGB_HPP
