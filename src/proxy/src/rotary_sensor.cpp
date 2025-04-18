#include "micras/proxy/rotary_sensor.hpp"
#include <random>
#include "box2d/box2d.h"

namespace micras::proxy {

RotarySensor::RotarySensor(const Config& config) :
    bodyId{config.bodyId},
    resolution{config.resolution},
    noise{config.noise},
    stopwatch{std::make_unique<Stopwatch>(Stopwatch::Config{})} {
}

void RotarySensor::update() {
    // Get the body's angular velocity from Box2D
    float angular_velocity = b2Body_GetAngularVelocity(bodyId);
    
    // Add noise to simulate real encoder behavior
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::normal_distribution<float> noise_dist(0.0f, noise);
    
    // Calculate elapsed time in seconds
    float elapsed_time = stopwatch->elapsed_time_ms() / 1000.0f;
    stopwatch->reset_ms();
    
    // Update position based on angular velocity, resolution, and elapsed time
    float delta_position = angular_velocity * resolution * elapsed_time;
    position += delta_position + noise_dist(gen);
    
    // Update velocity
    velocity = angular_velocity;
}

float RotarySensor::get_position() const {
    return position;
}

float RotarySensor::get_velocity() const {
    return velocity;
}

void RotarySensor::reset() {
    position = 0.0f;
    velocity = 0.0f;
    last_position = 0.0f;
}

}  // namespace micras::proxy 
