#include "micras/proxy/fan.hpp"
#include "physics/box2d_motor.hpp"

#include <algorithm>

namespace micras::proxy {

Fan::Fan(const Config& config) : micrasBody{config.micrasBody} {
    this->stop();
}

void Fan::enable() {
    this->enabled = true;
    this->micrasBody->getLeftMotor().isFanOn = true;
    this->micrasBody->getRightMotor().isFanOn = true;
}

void Fan::disable() {
    this->enabled = false;
    this->micrasBody->getLeftMotor().isFanOn = false;
    this->micrasBody->getRightMotor().isFanOn = false;
}

void Fan::set_speed(float speed) {
    // Clamp speed to -100 to 100 range
    this->current_speed = std::clamp(speed, -100.0f, 100.0f);
}

float Fan::update() {
    return this->current_speed;
}

void Fan::stop() {
    this->current_speed = 0.0f;
}

float Fan::get_speed() const {
    return this->current_speed;
}

void Fan::set_direction(RotationDirection direction) {
    this->current_direction = direction;
}

}  // namespace micras::proxy
