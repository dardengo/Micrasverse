#ifndef MICRAS_PROXY_ROTARY_SENSOR_HPP
#define MICRAS_PROXY_ROTARY_SENSOR_HPP

#include "micras/proxy/stopwatch.hpp"
#include "box2d/box2d.h"
#include "physics/box2d_micrasbody.hpp"
#include "micrasverse_core/types.hpp"

#include <memory>

namespace micras::proxy {

class RotarySensor {
public:
    struct Config {
        micrasverse::physics::Box2DMicrasBody* micrasBody = nullptr;
        float resolution;  // Resolution of the encoder in counts per revolution
        float noise;       // Noise level in counts
        bool isLeftWheel;
    };

    explicit RotarySensor(const Config& config);

    float get_position();


private:
    micrasverse::physics::Box2DMicrasBody* micrasBody;
    b2BodyId bodyId;
    float resolution;
    float noise;
    float position{0.0f};
    float last_position{0.0f};
    micrasverse::types::Vec2  global_position{0.0f, 0.0f};
    micrasverse::types::Vec2 last_global_position{0.0f, 0.0f};
    std::unique_ptr<Stopwatch> stopwatch;
    bool isLeftWheel;
};

}  // namespace micras::proxy

#endif  // MICRAS_PROXY_ROTARY_SENSOR_HPP 
