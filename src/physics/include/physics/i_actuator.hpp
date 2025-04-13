#ifndef MICRASVERSE_PHYSICS_I_ACTUATOR_HPP
#define MICRASVERSE_PHYSICS_I_ACTUATOR_HPP

#include "micrasverse_core/types.hpp"

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
    virtual types::Vec2 getPosition() const = 0;
    
    // Command interface
    virtual void setCommand(float command) = 0;
    virtual float getCommand() const = 0;
    virtual bool isActive() const = 0;
    
    // Update method
    virtual void update(float deltaTime) = 0;
};

// Forward declarations
class IMotor;
class ILED;

} // namespace micrasverse::physics

#endif // MICRASVERSE_PHYSICS_I_ACTUATOR_HPP 
