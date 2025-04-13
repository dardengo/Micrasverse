#ifndef MICRASVERSE_PHYSICS_I_ROBOT_CONTROLLER_HPP
#define MICRASVERSE_PHYSICS_I_ROBOT_CONTROLLER_HPP

#include <vector>
#include <memory>
#include <string>

namespace micrasverse::physics {

// Forward declarations
class ISensor;
class IActuator;
class IRobot;

class IRobotController {
public:
    virtual ~IRobotController() = default;
    
    // Initialize with robot
    virtual void setRobot(IRobot* robot) = 0;
    
    // Controller lifecycle
    virtual void init() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void reset() = 0;
    
    // Controller configuration
    virtual void setParameter(const std::string& name, float value) = 0;
    virtual float getParameter(const std::string& name) const = 0;
};

} // namespace micrasverse::physics

#endif // MICRASVERSE_PHYSICS_I_ROBOT_CONTROLLER_HPP 
