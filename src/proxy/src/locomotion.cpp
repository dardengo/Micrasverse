#include "proxy/locomotion.hpp"

#include <cmath>

namespace micras::proxy {

Locomotion::Locomotion(const b2BodyId bodyId)
    : bodyId{bodyId},
    left_motor{bodyId, {-micrasverse::MICRAS_HALFWIDTH,  0.0f}, true},
    right_motor{bodyId, {micrasverse::MICRAS_HALFWIDTH, 0.0f}, false}
{
    this->stop();
    this->disable();
}

void Locomotion::enable() {

}

void Locomotion::disable() {

}

void Locomotion::attachMotor(b2Vec2 localPosition, bool leftWheel) {
    if (leftWheel) {
        this->left_motor = micrasverse::physics::Motor(this->bodyId, localPosition, leftWheel);
    } else {
        this->right_motor = micrasverse::physics::Motor(this->bodyId, localPosition, leftWheel);
    }
}

void Locomotion::update(float deltaTime, bool isFanOn) {
    this->left_motor.update(deltaTime, isFanOn);
    this->right_motor.update(deltaTime, isFanOn);
}

void Locomotion::setWheelCommand(float left_command, float right_command) {
    this->left_motor.setCommand(left_command);
    this->right_motor.setCommand(right_command);
}

void Locomotion::setCommand(float linear, float angular) {
    float left_command = linear - angular;
    float right_command = linear + angular;

    if (std::abs(left_command) > 100.0F) {
        left_command *= 100.0F / std::abs(left_command);
        right_command *= 100.0F / std::abs(left_command);
    }

    if (std::abs(right_command) > 100.0F) {
        left_command *= 100.0F / std::abs(right_command);
        right_command *= 100.0F / std::abs(right_command);
    }

    this->setWheelCommand(left_command, right_command);
}

void Locomotion::stop() {
    this->setWheelCommand(0.0F, 0.0F);
}

}  // namespace micras::proxy
