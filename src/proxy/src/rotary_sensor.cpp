#include "micras/proxy/rotary_sensor.hpp"
#include "constants.hpp"
#include "physics/box2d_motor.hpp"
#include <random>

namespace micras::proxy {

RotarySensor::RotarySensor(const Config& config) :
    micrasBody{config.micrasBody},
    resolution{config.resolution},
    noise{config.noise},
    stopwatch{std::make_unique<Stopwatch>(Stopwatch::Config{})},
    isLeftWheel{config.isLeftWheel} {
    bodyId = micrasBody->getBodyId();
}

float RotarySensor::get_position() {

    if (isLeftWheel) {
        this->global_position = micrasBody->getLeftMotor().getPosition();
    } else {
        this->global_position = micrasBody->getRightMotor().getPosition();
    }

    micrasverse::types::Vec2 delta_position = this->global_position - this->last_global_position;
    this->last_global_position = this->global_position;
    float distance = delta_position.length();
    this->position += distance / micrasverse::MICRAS_WHEEL_RADIUS;

    return this->position;
}

}  // namespace micras::proxy 
