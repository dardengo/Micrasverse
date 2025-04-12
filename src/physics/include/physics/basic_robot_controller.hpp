#ifndef BASIC_ROBOT_CONTROLLER_HPP
#define BASIC_ROBOT_CONTROLLER_HPP

#include "physics/i_robot_controller.hpp"
#include <unordered_map>
#include <string>

namespace micrasverse::physics {

class BasicRobotController : public IRobotController {
public:
    BasicRobotController();
    virtual ~BasicRobotController() = default;

    // IRobotController interface implementation
    void setRobot(IRobot* robot) override;
    
    void init() override;
    void update(float deltaTime) override;
    void reset() override;
    
    void setParameter(const std::string& name, float value) override;
    float getParameter(const std::string& name) const override;

private:
    IRobot* robot;
    std::unordered_map<std::string, float> parameters;
    
    void updateMotors(float deltaTime);
    void updateLEDs(float deltaTime);
};

} // namespace micrasverse::physics

#endif // BASIC_ROBOT_CONTROLLER_HPP 
