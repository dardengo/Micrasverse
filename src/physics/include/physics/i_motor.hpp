#ifndef MICRASVERSE_PHYSICS_I_MOTOR_HPP
#define MICRASVERSE_PHYSICS_I_MOTOR_HPP

#include "physics/i_actuator.hpp"
#include "micrasverse_core/types.hpp"

namespace micrasverse::physics {

class IMotor : public IActuator {
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

    virtual float getCommand() const = 0;
    virtual bool isActive() const = 0;
    virtual ActuatorType getType() const override { return ActuatorType::MOTOR; }
};

} // namespace micrasverse::physics

#endif // MICRASVERSE_PHYSICS_I_MOTOR_HPP 
