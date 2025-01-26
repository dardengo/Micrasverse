#ifndef MICRAS_PROXY_ARGB_HPP
#define MICRAS_PROXY_ARGB_HPP

#include "physics/argb.hpp"

#include "box2d/box2d.h"
#include "glm/glm.hpp"

#include <vector>
#include <array>
#include <cstdint>

namespace micrasverse::proxy {

template <uint8_t num_of_leds>
class TArgb {
public:
    std::vector<micrasverse::physics::Argb> argbs;

    struct Config {
        float max_brightness;
    };

    struct Color {
        uint8_t red;
        uint8_t green;
        uint8_t blue;

        Color operator*(float brightness) const {
            return {
                static_cast<uint8_t>(this->red * brightness),
                static_cast<uint8_t>(this->green * brightness),
                static_cast<uint8_t>(this->blue * brightness),
            };
        }
    };

    struct Colors {
        Colors() = delete;

        static constexpr Color red{255, 0, 0};
        static constexpr Color green{0, 255, 0};
        static constexpr Color blue{0, 0, 255};
        static constexpr Color yellow{255, 255, 0};
        static constexpr Color cyan{0, 255, 255};
        static constexpr Color magenta{255, 0, 255};
        static constexpr Color white{255, 255, 255};
    };

    explicit TArgb(const Config& config, b2BodyId bodyId);

    void attachArgb(b2Vec2 localPosition, b2Vec2 size, glm::vec3 color);

    void set_color(const Color& color, uint8_t index);

    void set_color(const Color& color);

    void set_colors(const std::array<Color, num_of_leds>& colors);

    void turn_off(uint8_t index);

    void turn_off();

private:
    float brightness;

    b2BodyId bodyId;

    void encode_color(const Color& color, uint8_t index);
};
}  // namespace micrasverse::proxy

#include "../../src/argb.cpp"

#endif  // MICRAS_PROXY_ARGB_HPP
