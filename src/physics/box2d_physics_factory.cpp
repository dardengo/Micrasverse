#include "physics/box2d_physics_factory.hpp"
#include "physics/argb.hpp"
#include "physics/dipswitch.hpp"
#include "physics/distancesensor.hpp"
#include "physics/motor.hpp"
#include "config/constants.hpp"

namespace micrasverse::physics {

Box2DPhysicsFactory::Box2DPhysicsFactory(b2BodyId bodyId) : bodyId(bodyId) {
}

std::unique_ptr<IARGB> Box2DPhysicsFactory::createARGB(
    const core::Vec2& localPosition, 
    const core::Vec2& size, 
    const core::Color& color) {
    
    b2Vec2 b2LocalPosition = {localPosition.x, localPosition.y};
    b2Vec2 b2Size = {size.x, size.y};
    core::Color colorCopy = color; // Need a non-const copy
    
    return std::make_unique<Argb>(bodyId, b2LocalPosition, b2Size, colorCopy);
}

std::unique_ptr<IDipSwitch> Box2DPhysicsFactory::createDipSwitch(size_t numSwitches) {
    return std::make_unique<DipSwitch>(numSwitches);
}

std::unique_ptr<IDistanceSensor> Box2DPhysicsFactory::createDistanceSensor(
    const core::Vec2& localPosition, 
    float angle) {
    
    b2Vec2 b2LocalPosition = {localPosition.x, localPosition.y};
    
    return std::make_unique<DistanceSensor>(bodyId, b2LocalPosition, angle);
}

std::unique_ptr<IMotor> Box2DPhysicsFactory::createMotor(
    const core::Vec2& localPosition, 
    bool leftWheel, 
    float angle,
    float resistance, 
    float ke, 
    float kt, 
    float maxVoltage) {
    
    b2Vec2 b2LocalPosition = {localPosition.x, localPosition.y};
    
    return std::make_unique<Motor>(
        bodyId, 
        b2LocalPosition, 
        leftWheel, 
        angle, 
        resistance != 0.0f ? resistance : MOTOR_RESISTANCE,
        ke != 0.0f ? ke : MOTOR_KE,
        kt != 0.0f ? kt : MOTOR_KT,
        maxVoltage != 0.0f ? maxVoltage : MOTOR_MAX_VOLTAGE
    );
}

} // namespace micrasverse::physics 
