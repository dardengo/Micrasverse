#ifndef BOX2D_ROBOT_HPP
#define BOX2D_ROBOT_HPP

#include "physics/i_robot.hpp"
#include "box2d/box2d.h"
#include <memory>
#include <vector>
#include <string>
#include "micrasverse_core/types.hpp"
namespace micrasverse::physics {

class Box2DRobot : public IRobot {
public:
    Box2DRobot(b2WorldId worldId, const types::Vec2& position, const types::Vec2& size);
    virtual ~Box2DRobot();

    // IRobot interface implementation
    types::Vec2 getPosition() const override;
    float getRotation() const override;
    types::Vec2 getSize() const override;
    
    float getLinearVelocity() const override;
    float getAngularVelocity() const override;
    float getLinearAcceleration() const override;

    const std::vector<std::shared_ptr<ISensor>>& getSensors() const override;
    const std::vector<std::shared_ptr<IActuator>>& getActuators() const override;
    
    IRobotController* getController() override;
    void setController(std::shared_ptr<IRobotController> controller) override;

    void update(float deltaTime) override;
    void reset() override;

    // Box2D specific methods
    b2BodyId getBodyId() const;
    void createBody(b2WorldId worldId, const types::Vec2& position);

private:
    // Robot physical properties
    b2BodyId bodyId;
    types::Vec2 size;
    float linearAcceleration;
    
    // Hardware components
    std::vector<std::shared_ptr<ISensor>> sensors;
    std::vector<std::shared_ptr<IActuator>> actuators;
    
    // Controller
    std::shared_ptr<IRobotController> controller;
    
    // Setup methods
    void setupDistanceSensors();
    void setupMotors();
    void setupLEDs();
};

} // namespace micrasverse::physics

#endif // BOX2D_ROBOT_HPP 
