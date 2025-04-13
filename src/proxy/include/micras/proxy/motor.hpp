#ifndef MICRAS_PROXY_MOTOR_HPP
#define MICRAS_PROXY_MOTOR_HPP

#include "micras/proxy/stopwatch.hpp"
#include "box2d/box2d.h"
#include <memory>

namespace micras::proxy {

class Motor {
public:
    struct Config {
        float max_speed = 100.0f;  // Maximum speed in RPM
        float max_torque = 1.0f;   // Maximum torque in Nm
        float gear_ratio = 1.0f;   // Gear ratio
    };

    Motor(const Config& config, const b2BodyId bodyId);

    void set_speed(float speed);

    float get_speed() const;

    void update();

private:
    b2BodyId bodyId;
    float max_speed;
    float max_torque;
    float gear_ratio;
    float current_speed = 0.0f;
    std::unique_ptr<Stopwatch> stopwatch;
};

}  // namespace micras::proxy

#endif  // MICRAS_PROXY_MOTOR_HPP 
