#ifndef MOTOR_HPP
#define MOTOR_HPP

#include "core/constants.hpp"

#include "box2d/box2d.h"
#include <algorithm>

namespace micrasverse::physics {

class Motor {
private:
    float resistance;       // Motor resistance (R)
    float ke;               // Back EMF constant
    float kt;               // Torque constant
    float maxVoltage;       // Maximum input voltage
    float inputCommand;     // Motor command (-100 to +100)
    float current;          // Current through the motor
    float angularVelocity;  // Angular velocity of the rotor

    float frictionCoefficient; // Coefficient of friction (mu)
    float bodyMass;            // Mass of the body

    b2BodyId bodyId;        // Body to apply the force to
    b2Vec2 localPosition;   // Local position of the motor on the body
    b2Vec2 localDirection;  // Local direction of the motor on the body
    float angle;            // Angle of the motor on the body


public:
    float bodyLinearVelocity;   // Linear velocity of the body
    float t_angle;              // Angle of the motor on the body
    float bodyAngularVelocity;  // Angular velocity of the body
    bool leftWheel;             // Is this a left wheel?
    Motor(b2BodyId bodyId, b2Vec2 localPosition, bool leftWheel, const float angle = B2_PI / 2.0f, float R = MOTOR_RESISTANCE, float ke = MOTOR_KE, float kt = MOTOR_KT, float maxVoltage = MOTOR_MAX_VOLTAGE);

    // Set the motor command (0-100)

    void update(float deltaTime);


};

} // namespace micrasverse::physics


#endif // MOTOR_HPP
