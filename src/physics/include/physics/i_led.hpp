#ifndef MICRASVERSE_PHYSICS_I_LED_HPP
#define MICRASVERSE_PHYSICS_I_LED_HPP

#include "physics/i_actuator.hpp"
#include "micrasverse_core/types.hpp"

namespace micrasverse::physics {

class ILED : public IActuator {
public:
    virtual ~ILED() = default;
    
    // LED-specific methods
    virtual void setColor(const types::Color& color) = 0;
    virtual types::Color getColor() const = 0;
    virtual void turnOn() = 0;
    virtual void turnOn(const types::Color& color) = 0;
    virtual void turnOff() = 0;
    virtual void toggle() = 0;
    virtual bool isOn() const = 0;
    virtual types::Vec2 getSize() const = 0;
    
    // Override the getType method from IActuator
    virtual ActuatorType getType() const override { return ActuatorType::LED; }
};

} // namespace micrasverse::physics

#endif // MICRASVERSE_PHYSICS_I_LED_HPP 
