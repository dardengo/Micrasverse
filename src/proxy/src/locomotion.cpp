#include "micras/proxy/locomotion.hpp"
#include "physics/box2d_motor.hpp"

#include <cmath>

namespace micras::proxy {

Locomotion::Locomotion(const Config& config) : micrasBody(config.micrasBody) {
    this->stop();
    this->disable();
}

void Locomotion::enable() {
    // Set both motors to active state
    if (micrasBody) {
        micrasBody->getLeftMotor().setCommand(0.0f);  // Initialize with zero command
        micrasBody->getRightMotor().setCommand(0.0f);  // Initialize with zero command
    }
}

void Locomotion::disable() {
    // Stop and deactivate both motors
    if (micrasBody) {
        micrasBody->getLeftMotor().setCommand(0.0f);
        micrasBody->getRightMotor().setCommand(0.0f);
    }
}

void Locomotion::update(float deltaTime, bool isFanOn) {
    // No-op since micrasBody handles motor updates
}

void Locomotion::set_wheel_command(float left_command, float right_command) {
    if (micrasBody) {
        micrasBody->getLeftMotor().setCommand(left_command);
        micrasBody->getRightMotor().setCommand(right_command);
    }
}

void Locomotion::set_command(float linear, float angular) {
    if (micrasBody) {
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
}

void Locomotion::stop() {
    this->set_wheel_command(0.0F, 0.0F);
}

}  // namespace micras::proxy
