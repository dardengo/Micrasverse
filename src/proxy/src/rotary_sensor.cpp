#include "micras/proxy/rotary_sensor.hpp"
#include "constants.hpp"
#include "physics/box2d_motor.hpp"
#include "box2d/box2d.h"
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
    float angular_velocity = 0.0f;
    if (isLeftWheel) {
        this->global_position = micrasBody->getLeftMotor().getPosition();
    } else {
        this->global_position = micrasBody->getRightMotor().getPosition();
    }
    
    micrasverse::types::Vec2 delta_position = this->global_position - this->last_global_position;
    this->last_global_position = this->global_position;
    
    float distance_sign = b2Dot((b2Vec2){delta_position.x, delta_position.y}, micrasBody->linearVelocity);

    float distance = std::copysignf(delta_position.length(), distance_sign);
    
    
    this->position += distance / micrasverse::MICRAS_WHEEL_RADIUS;

    return this->position;
}

}  // namespace micras::proxy 
