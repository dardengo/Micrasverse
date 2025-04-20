#ifndef MICRAS_PROXY_ROTARY_SENSOR_HPP
#define MICRAS_PROXY_ROTARY_SENSOR_HPP

#include "micras/proxy/stopwatch.hpp"
#include "box2d/box2d.h"
#include "physics/box2d_micrasbody.hpp"

#include <memory>

namespace micras::proxy {

class RotarySensor {
public:
    struct Config {
        micrasverse::physics::Box2DMicrasBody* micrasBody = nullptr;
        float resolution;  // Resolution of the encoder in counts per revolution
        float noise;       // Noise level in counts
    };

    explicit RotarySensor(const Config& config);

    void update();

    float get_position() const;

    float get_velocity() const;

    void reset();

private:
    micrasverse::physics::Box2DMicrasBody* micrasBody;
    b2BodyId bodyId;
    float resolution;
    float noise;
    float position = 0.0f;
    float velocity = 0.0f;
    float last_position = 0.0f;
    std::unique_ptr<Stopwatch> stopwatch;
};

}  // namespace micras::proxy

#endif  // MICRAS_PROXY_ROTARY_SENSOR_HPP 
