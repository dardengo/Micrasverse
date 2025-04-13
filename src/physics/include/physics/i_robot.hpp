#ifndef MICRASVERSE_PHYSICS_I_ROBOT_HPP
#define MICRASVERSE_PHYSICS_I_ROBOT_HPP

#include "micrasverse_core/types.hpp"
#include <vector>
#include <memory>

namespace micrasverse::physics {

// Forward declarations
class ISensor;
class IActuator;
class IRobotController;

class IRobot {
public:
    virtual ~IRobot() = default;

    // Position and orientation
    virtual types::Vec2 getPosition() const = 0;
    virtual float getRotation() const = 0;
    virtual types::Vec2 getSize() const = 0;
    
    // Physics properties
    virtual float getLinearVelocity() const = 0;
    virtual float getAngularVelocity() const = 0;
    virtual float getLinearAcceleration() const = 0;

    // Hardware access
    virtual const std::vector<std::shared_ptr<ISensor>>& getSensors() const = 0;
    virtual const std::vector<std::shared_ptr<IActuator>>& getActuators() const = 0;
    
    // Controller
    virtual IRobotController* getController() = 0;
    virtual void setController(std::shared_ptr<IRobotController> controller) = 0;

    // Update robot state
    virtual void update(float deltaTime) = 0;
    virtual void reset() = 0;
};

} // namespace micrasverse::physics

#endif // MICRASVERSE_PHYSICS_I_ROBOT_HPP 
