#ifndef MICRAS_PROXY_MOTOR_HPP
#define MICRAS_PROXY_MOTOR_HPP

#include "micras/proxy/stopwatch.hpp"
#include "physics/box2d_micrasbody.hpp"
#include <memory>

namespace micras::proxy {

class Motor {
public:
    struct Config {
        micrasverse::physics::Box2DMicrasBody* micrasBody;
        bool isLeftWheel;
        float max_speed;  // Maximum speed in RPM
        float max_torque;   // Maximum torque in Nm
        float gear_ratio;   // Gear ratio
    };

    explicit Motor(const Config& config);

    void set_speed(float speed);

    float get_speed() const;

    void update();

private:
    micrasverse::physics::Box2DMicrasBody* micrasBody;
    bool isLeftWheel;
    float max_speed;
    float max_torque;
    float gear_ratio;
    float current_speed = 0.0f;
    std::unique_ptr<Stopwatch> stopwatch;
};

}  // namespace micras::proxy

#endif  // MICRAS_PROXY_MOTOR_HPP 
