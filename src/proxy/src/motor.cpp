#include "micras/proxy/motor.hpp"
#include "physics/box2d_micrasbody.hpp"
#include "physics/box2d_motor.hpp"
#include <algorithm>
namespace micras::proxy {

Motor::Motor(const Config& config) :
    micrasBody{config.micrasBody},
    isLeftWheel{config.isLeftWheel},
    max_speed{config.max_speed},
    max_torque{config.max_torque},
    gear_ratio{config.gear_ratio},
    stopwatch{std::make_unique<Stopwatch>(Stopwatch::Config{})} {
}

void Motor::set_speed(float speed) {
    // Clamp speed to max_speed
    current_speed = std::clamp(speed, -max_speed, max_speed);
    
    // Set the motor command in Box2DMicrasBody
    if (isLeftWheel) {
        micrasBody->getLeftMotor().setCommand(current_speed);
    } else {
        micrasBody->getRightMotor().setCommand(current_speed);
    }
}

float Motor::get_speed() const {
    return current_speed;
}

void Motor::update() {
    // The Box2DMicrasBody already updates its motors in its update method
}

}  // namespace micras::proxy 
