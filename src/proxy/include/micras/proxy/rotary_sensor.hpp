#ifndef MICRAS_PROXY_ROTARY_SENSOR_HPP
#define MICRAS_PROXY_ROTARY_SENSOR_HPP

#include "micras/proxy/stopwatch.hpp"
#include "box2d/box2d.h"
#include <memory>

namespace micras::proxy {

class RotarySensor {
public:
    struct Config {
        float resolution = 1.0f;  // Resolution of the encoder in counts per revolution
        float noise = 0.0f;       // Noise level in counts
    };

    RotarySensor(const Config& config, const b2BodyId bodyId);

    void update();

    float get_position() const;

    float get_velocity() const;

    void reset();

private:
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
