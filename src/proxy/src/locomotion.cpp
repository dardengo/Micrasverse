#include "micras/proxy/locomotion.hpp"

#include <cmath>

namespace micras::proxy {

Locomotion::Locomotion(const Config& config)
    : bodyId{config.bodyId}
    , left_motor{config.left_motor}
    , right_motor{config.right_motor}
{
    this->stop();
    this->disable();
}

void Locomotion::enable() {
    // Set both motors to active state
    if (left_motor) {
        left_motor->setCommand(0.0f);  // Initialize with zero command
    }
    if (right_motor) {
        right_motor->setCommand(0.0f);  // Initialize with zero command
    }
}

void Locomotion::disable() {
    // Stop and deactivate both motors
    if (left_motor) {
        left_motor->setCommand(0.0f);
    }
    if (right_motor) {
        right_motor->setCommand(0.0f);
    }
}

void Locomotion::update(float deltaTime, bool isFanOn) {
    if (left_motor) {
        left_motor->update(deltaTime, isFanOn);
    }
    if (right_motor) {
        right_motor->update(deltaTime, isFanOn);
    }
}

void Locomotion::set_wheel_command(float left_command, float right_command) {
    if (left_motor) {
        left_motor->setCommand(left_command);
    }
    if (right_motor) {
        right_motor->setCommand(right_command);
    }
}

void Locomotion::set_command(float linear, float angular) {
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

    this->set_wheel_command(left_command, right_command);
}

void Locomotion::stop() {
    this->set_wheel_command(0.0F, 0.0F);
}

}  // namespace micras::proxy
