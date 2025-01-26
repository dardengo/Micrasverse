#ifndef MICRAS_PROXY_LOCOMOTION_HPP
#define MICRAS_PROXY_LOCOMOTION_HPP

#include "physics/motor.hpp"
#include "box2d/box2d.h"

namespace micrasverse::proxy {

class Locomotion {
public:

    explicit Locomotion(const b2BodyId bodyId);

    void enable();

    void disable();

    void attachMotor(b2Vec2 localPosition, bool leftWheel);

    void update(float deltaTime, bool isFanOn);

    void setWheelCommand(float left_command, float right_command);

    void setCommand(float linear, float angular);

    void stop();

    physics::Motor left_motor;
    physics::Motor right_motor;

private:
    b2BodyId bodyId;
};

}  // namespace micras::proxy

#endif  // MICRAS_PROXY_LOCOMOTION_HPP
