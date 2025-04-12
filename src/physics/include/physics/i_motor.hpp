#ifndef I_MOTOR_HPP
#define I_MOTOR_HPP

#include "core/types.hpp"

namespace micrasverse::physics {

class IMotor {
public:
    virtual ~IMotor() = default;

    // Set the motor command (0-100)
    virtual void setCommand(float command) = 0;

    // Update the motor simulation
    virtual void update(float deltaTime, bool isFanOn) = 0;

    // Accessor methods
    virtual float getCurrent() const = 0;
    virtual float getAngularVelocity() const = 0;
    virtual float getAppliedForce() const = 0;
    virtual float getTorque() const = 0;
    virtual float getBodyLinearVelocity() const = 0;
    virtual float getBodyAngularVelocity() const = 0;
    virtual bool isLeftWheel() const = 0;
    virtual bool getFanState() const = 0;
};

} // namespace micrasverse::physics

#endif // I_MOTOR_HPP 
