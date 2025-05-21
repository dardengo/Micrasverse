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

float RotarySensor::get_position() const {
    auto modifiable = const_cast<RotarySensor*>(this);

    float angular_velocity = 0.0f;
    if (isLeftWheel) {
        modifiable->global_position = micrasBody->getLeftMotor().getPosition();
    } else {
        modifiable->global_position = micrasBody->getRightMotor().getPosition();
    }

    micrasverse::types::Vec2 delta_position = modifiable->global_position - modifiable->last_global_position;
    modifiable->last_global_position = modifiable->global_position;

    auto  forward_direction = b2Rot_GetYAxis(b2Body_GetRotation(micrasBody->getBodyId()));
    float distance_sign = b2Dot((b2Vec2){delta_position.x, delta_position.y}, forward_direction);

    float distance = std::copysignf(delta_position.length(), distance_sign);

    modifiable->position += distance / micrasverse::MICRAS_WHEEL_RADIUS;

    return modifiable->position;
}

}  // namespace micras::proxy
