#ifndef BOX2D_PHYSICS_FACTORY_HPP
#define BOX2D_PHYSICS_FACTORY_HPP

#include "physics/i_physics_factory.hpp"
#include "box2d/box2d.h"

namespace micrasverse::physics {

class Box2DPhysicsFactory : public IPhysicsFactory {
public:
    explicit Box2DPhysicsFactory(b2BodyId bodyId);

    // Implementation of factory methods
    std::unique_ptr<IARGB> createARGB(
        const core::Vec2& localPosition, 
        const core::Vec2& size, 
        const core::Color& color) override;

    std::unique_ptr<IDipSwitch> createDipSwitch(size_t numSwitches) override;

    std::unique_ptr<IDistanceSensor> createDistanceSensor(
        const core::Vec2& localPosition, 
        float angle) override;

    std::unique_ptr<IMotor> createMotor(
        const core::Vec2& localPosition, 
        bool leftWheel, 
        float angle,
        float resistance, 
        float ke, 
        float kt, 
        float maxVoltage) override;

private:
    b2BodyId bodyId;
};

} // namespace micrasverse::physics

#endif // BOX2D_PHYSICS_FACTORY_HPP 
