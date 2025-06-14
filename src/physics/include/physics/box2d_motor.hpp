#ifndef BOX2D_MOTOR_HPP
#define BOX2D_MOTOR_HPP

#include "box2d/box2d.h"
#include "constants.hpp"
#include "micrasverse_core/types.hpp"

namespace micrasverse::physics {

class Box2DMotor {
public:
    Box2DMotor(
        b2BodyId bodyId, const types::Vec2& localPosition, bool leftWheel, float angle = B2_PI / 2.0f, float R = MOTOR_RESISTANCE,
        float ke = MOTOR_KE, float kt = MOTOR_KT, float maxCommandVoltage = MOTOR_MAX_COMMAND_VOLTAGE, float nominalVoltage = MOTOR_NOMINAL_VOLTAGE
    );
    virtual ~Box2DMotor() = default;

    types::Vec2 getPosition() const;

    void setCommand(float command);

    float getCommand() const { return inputCommand; }

    bool isActive() const { return std::abs(inputCommand) > 0.1f; }

    void update(float deltaTime) { update(deltaTime, isFanOn); }

    void update(float deltaTime, bool isFanOn);

    float getCurrent() const { return current; }

    float getAngularVelocity() const { return rotorAngularVelocity; }

    float getAppliedForce() const { return appliedForce; }

    float getTorque() const { return torque; }

    float getBodyLinearVelocity() const { return bodyLinearVelocity; }

    float getBodyAngularVelocity() const { return bodyAngularVelocity; }

    bool isLeftWheel() const { return leftWheel; }

    bool getFanState() const { return isFanOn; }

    bool isFanOn{false};  // Is the fan on?

private:
    float resistance;            // Motor resistance (R)
    float ke;                    // Back EMF constant
    float kt;                    // Torque constant
    float nominalVoltage;        // Nominal voltage of the motor
    float maxCommandVoltage;     // Maximum input voltage
    float inputCommand;          // Motor command (-100 to +100)
    float current;               // Current through the motor
    float rotorAngularVelocity;  // Angular velocity of the rotor
    float frictionCoefficient;   // Coefficient of friction (mu)
    float bodyMass;              // Mass of the body
    float appliedForce;          // Force applied by the motor
    float torque;                // Torque generated by the motor
    float bodyLinearVelocity;    // Linear velocity of the body
    float bodyAngularVelocity;   // Angular velocity of the body
    bool  leftWheel;             // Is this a left wheel?

    // Box2D specific properties
    b2BodyId bodyId;          // Body to apply the force to
    b2Vec2   localPosition;   // Changed from types::Vec2 to b2Vec2
    b2Vec2   localDirection;  // Changed from types::Vec2 to b2Vec2
    float    angle;           // Angle of the motor on the body
};

}  // namespace micrasverse::physics

#endif  // BOX2D_MOTOR_HPP
