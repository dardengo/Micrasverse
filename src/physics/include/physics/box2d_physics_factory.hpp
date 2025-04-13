#ifndef BOX2D_PHYSICS_FACTORY_HPP
#define BOX2D_PHYSICS_FACTORY_HPP

#include "physics/i_physics_factory.hpp"
#include "box2d/box2d.h"
#include "micrasverse_core/types.hpp"


namespace micrasverse::physics {

class Box2DPhysicsFactory : public IPhysicsFactory {
public:
    Box2DPhysicsFactory(b2WorldId worldId, b2BodyId bodyId) 
        : worldId(worldId), bodyId(bodyId) {}

    // Implementation of factory methods
    std::unique_ptr<IArgb> createARGB(
        const types::Vec2& localPosition, 
        const types::Vec2& size, 
        const types::Color& color) override;

    std::unique_ptr<IDipSwitch> createDipSwitch(size_t numSwitches) override;

    std::unique_ptr<IDistanceSensor> createDistanceSensor(
        const types::Vec2& localPosition, 
        float angle) override;

    std::unique_ptr<IMotor> createMotor(
        const types::Vec2& localPosition, 
        bool leftWheel, 
        float angle,
        float resistance, 
        float ke, 
        float kt, 
        float maxVoltage) override;

private:
    b2WorldId worldId;
    b2BodyId bodyId;
};

} // namespace micrasverse::physics

#endif // BOX2D_PHYSICS_FACTORY_HPP 
