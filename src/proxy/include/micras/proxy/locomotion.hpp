#ifndef MICRAS_PROXY_LOCOMOTION_HPP
#define MICRAS_PROXY_LOCOMOTION_HPP

#include "physics/box2d_motor.hpp"
#include "box2d/box2d.h"
#include <memory>

namespace micras::proxy {

class Locomotion {
public:
    struct Config {
        b2BodyId bodyId;
        std::shared_ptr<micrasverse::physics::Box2DMotor> left_motor;
        std::shared_ptr<micrasverse::physics::Box2DMotor> right_motor;
    };

    explicit Locomotion(const Config& config);

    void enable();

    void disable();

    void update(float deltaTime, bool isFanOn);

    void set_wheel_command(float left_command, float right_command);

    void set_command(float linear, float angular);

    void stop();

    std::shared_ptr<micrasverse::physics::Box2DMotor> left_motor;
    std::shared_ptr<micrasverse::physics::Box2DMotor> right_motor;

private:
    b2BodyId bodyId;
};

}  // namespace micras::proxy

#endif  // MICRAS_PROXY_LOCOMOTION_HPP
