#ifndef I_ACTUATOR_HPP
#define I_ACTUATOR_HPP

#include "core/types.hpp"

namespace micrasverse::physics {

enum class ActuatorType {
    MOTOR,
    LED,
    DISPLAY,
    SPEAKER
};

class IActuator {
public:
    virtual ~IActuator() = default;
    
    // Actuator basics
    virtual ActuatorType getType() const = 0;
    virtual core::Vec2 getPosition() const = 0;
    
    // Control
    virtual void setCommand(float command) = 0;
    virtual float getCommand() const = 0;
    
    // Status
    virtual bool isActive() const = 0;
    virtual void update(float deltaTime) = 0;
};

// Motor-specific interface
class IMotor : public IActuator {
public:
    virtual ~IMotor() = default;
    
    // Motor-specific methods
    virtual float getCurrent() const = 0;
    virtual float getAngularVelocity() const = 0;
    virtual float getAppliedForce() const = 0;
    virtual float getTorque() const = 0;
    virtual float getBodyLinearVelocity() const = 0;
    virtual float getBodyAngularVelocity() const = 0;
    virtual bool isLeftWheel() const = 0;
    virtual bool getFanState() const = 0;
};

// LED-specific interface
class ILED : public IActuator {
public:
    virtual ~ILED() = default;
    
    // LED-specific methods
    virtual void setColor(const core::Color& color) = 0;
    virtual core::Color getColor() const = 0;
    virtual void turnOn() = 0;
    virtual void turnOn(const core::Color& color) = 0;
    virtual void turnOff() = 0;
    virtual void toggle() = 0;
    virtual bool isOn() const = 0;
    virtual core::Vec2 getSize() const = 0;
};

} // namespace micrasverse::physics

#endif // I_ACTUATOR_HPP 
