#include "micras/proxy/fan.hpp"
#include <algorithm>

namespace micras::proxy {

Fan::Fan(const Config& config) :
    micrasBody{config.micrasBody},
    max_acceleration{config.max_acceleration},
    max_speed{config.max_speed},
    stopwatch{std::make_unique<Stopwatch>(Stopwatch::Config{})} {
    this->stop();
}

void Fan::enable() {
    this->enabled = true;
}

void Fan::disable() {
    this->enabled = false;
    this->stop();
}

void Fan::set_speed(float speed) {
    // Clamp speed to -100 to 100 range
    this->target_speed = std::clamp(speed, -100.0f, 100.0f);
}

float Fan::update() {
    if (!this->enabled) {
        return 0.0f;
    }

    float elapsed_time = stopwatch->elapsed_time_ms() / 1000.0f; // Convert to seconds
    stopwatch->reset_ms();

    // Calculate acceleration based on elapsed time
    float acceleration = this->max_acceleration * elapsed_time;
    
    // Move current speed towards target speed
    if (this->current_speed < this->target_speed) {
        this->current_speed = std::min(this->current_speed + acceleration, this->target_speed);
    } else if (this->current_speed > this->target_speed) {
        this->current_speed = std::max(this->current_speed - acceleration, this->target_speed);
    }

    // Set direction based on speed
    if (this->current_speed > 0.0f) {
        this->set_direction(RotationDirection::FORWARD);
    } else if (this->current_speed < 0.0f) {
        this->set_direction(RotationDirection::BACKWARDS);
    }

    return this->current_speed;
}

void Fan::stop() {
    this->current_speed = 0.0f;
    this->target_speed = 0.0f;
}

float Fan::get_speed() const {
    return this->current_speed;
}

void Fan::set_direction(RotationDirection direction) {
    this->current_direction = direction;
}

}  // namespace micras::proxy 
