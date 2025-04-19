#include "micras/proxy/motor.hpp"
#include "box2d/box2d.h"
#include <algorithm>
namespace micras::proxy {

Motor::Motor(const Config& config) :
    bodyId{config.bodyId},
    worldId{config.worldId},
    max_speed{config.max_speed},
    max_torque{config.max_torque},
    gear_ratio{config.gear_ratio},
    stopwatch{std::make_unique<Stopwatch>(Stopwatch::Config{})} {
}

void Motor::set_speed(float speed) {
    // Clamp speed to max_speed
    current_speed = std::clamp(speed, -max_speed, max_speed);
}

float Motor::get_speed() const {
    return current_speed;
}

void Motor::update() {
    // Get the current angular velocity
    float current_angular_velocity = b2Body_GetAngularVelocity(bodyId);
    
    // Calculate elapsed time in seconds
    float elapsed_time = stopwatch->elapsed_time_ms() / 1000.0f;
    stopwatch->reset_ms();
    
    // Calculate the torque to apply based on the current speed and gear ratio
    float target_angular_velocity = current_speed * gear_ratio;
    float angular_velocity_error = target_angular_velocity - current_angular_velocity;
    
    // Apply a proportional control to reach the target speed
    float torque = angular_velocity_error * max_torque * elapsed_time;
    torque = std::clamp(torque, -max_torque, max_torque);
    
    // Apply the torque to the body
    b2Body_ApplyTorque(bodyId, torque, true);
}

}  // namespace micras::proxy 
