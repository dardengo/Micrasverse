#include "physics/basic_robot_controller.hpp"
#include "physics/i_robot.hpp"
#include "physics/i_actuator.hpp"
#include "physics/i_sensor.hpp"
#include <algorithm>

namespace micrasverse::physics {

BasicRobotController::BasicRobotController() : robot(nullptr) {
    // Initialize default parameters
    parameters["left_motor_speed"] = 0.0f;
    parameters["right_motor_speed"] = 0.0f;
    parameters["led_brightness"] = 1.0f;
}

void BasicRobotController::setRobot(IRobot* robot) {
    this->robot = robot;
}

void BasicRobotController::init() {
    // Make sure we have a robot
    if (!robot) return;
    
    // Initialize the robot's actuators
    reset();
}

void BasicRobotController::update(float deltaTime) {
    // Make sure we have a robot
    if (!robot) return;
    
    // Update actuators based on parameters
    updateMotors(deltaTime);
    updateLEDs(deltaTime);
}

void BasicRobotController::reset() {
    // Reset all parameters to default values
    parameters["left_motor_speed"] = 0.0f;
    parameters["right_motor_speed"] = 0.0f;
    parameters["led_brightness"] = 1.0f;
    
    // Make sure all motors are stopped
    if (robot) {
        for (const auto& actuator : robot->getActuators()) {
            if (actuator->getType() == ActuatorType::MOTOR) {
                actuator->setCommand(0.0f);
            }
        }
    }
}

void BasicRobotController::setParameter(const std::string& name, float value) {
    parameters[name] = value;
}

float BasicRobotController::getParameter(const std::string& name) const {
    if (parameters.find(name) != parameters.end()) {
        return parameters.at(name);
    }
    return 0.0f;
}

void BasicRobotController::updateMotors(float deltaTime) {
    // Get motor speed parameters
    float leftSpeed = parameters.at("left_motor_speed");
    float rightSpeed = parameters.at("right_motor_speed");
    
    // Find motor actuators and set their commands
    for (const auto& actuator : robot->getActuators()) {
        if (actuator->getType() == ActuatorType::MOTOR) {
            auto motor = std::dynamic_pointer_cast<IMotor>(actuator);
            if (motor) {
                if (motor->isLeftWheel()) {
                    motor->setCommand(leftSpeed);
                } else {
                    motor->setCommand(rightSpeed);
                }
            }
        }
    }
}

void BasicRobotController::updateLEDs(float deltaTime) {
    // Get LED brightness parameter
    float brightness = parameters.at("led_brightness");
    
    // Apply brightness to all LEDs
    for (const auto& actuator : robot->getActuators()) {
        if (actuator->getType() == ActuatorType::LED) {
            auto led = std::dynamic_pointer_cast<ILED>(actuator);
            if (led && led->isOn()) {
                // Get current color and adjust brightness
                core::Color color = led->getColor();
                color = color * brightness;
                led->setColor(color);
            }
        }
    }
}

} // namespace micrasverse::physics 
