#ifndef MICRAS_PROXY_ARGB_HPP
#define MICRAS_PROXY_ARGB_HPP

#include "micrasverse_core/types.hpp"
#include "micras/core/types.hpp"
#include "physics/argb.hpp"
#include "physics/box2d_micrasbody.hpp"

#include <vector>
#include <array>
#include <cstdint>
#include <memory>
#include <functional>

namespace micras::proxy {

template <uint8_t num_of_leds>
class TArgb {
public:
    // Changed from direct objects to references to the physical ARGBs
    std::vector<std::reference_wrapper<micrasverse::physics::Argb>> argbRefs;

    struct Config {
        micrasverse::physics::Box2DMicrasBody* micrasBody = nullptr;    
        float uncertainty;
        std::array<float, num_of_leds> brightness;
    };

    /**
     * @brief Predefined colors.
     */
    struct Colors {
        Colors() = delete;

        static constexpr micrasverse::types::Color red{255, 0, 0};
        static constexpr micrasverse::types::Color green{0, 255, 0};
        static constexpr micrasverse::types::Color blue{0, 0, 255};
        static constexpr micrasverse::types::Color yellow{255, 255, 0};
        static constexpr micrasverse::types::Color cyan{0, 255, 255};
        static constexpr micrasverse::types::Color magenta{255, 0, 255};
        static constexpr micrasverse::types::Color white{255, 255, 255};
    };

    explicit TArgb(const Config& config);

    ~TArgb();

    void turn_on();

    void turn_off();

    void update();

    void attachArgb(b2Vec2 localPosition, b2Vec2 size, micrasverse::types::Color color);

    void set_color(const micrasverse::types::Color& color, uint8_t index);

    void set_color(const micrasverse::types::Color& color);

    void set_colors(const std::array<micrasverse::types::Color, num_of_leds>& colors);

    void encode_color(const micrasverse::types::Color& color, uint8_t index);

private:
    micrasverse::physics::Box2DMicrasBody* micrasBody;
    float uncertainty;
    std::array<float, num_of_leds> brightness;
};
}  // namespace micras::proxy

#include "../src/argb.cpp"

#endif  // MICRAS_PROXY_ARGB_HPP
