#include "physics/box2d_physics_factory.hpp"
#include "constants.hpp"
#include "physics/argb.hpp"
#include "physics/box2d_dipswitch.hpp"
#include "physics/box2d_distance_sensor.hpp"
#include "physics/box2d_motor.hpp"

namespace micrasverse::physics {

std::unique_ptr<IArgb> Box2DPhysicsFactory::createARGB(
    const types::Vec2& localPosition, 
    const types::Vec2& size, 
    const types::Color& color) {
    
    b2Vec2 b2LocalPosition = {localPosition.x, localPosition.y};
    b2Vec2 b2Size = {size.x, size.y};
    types::Color colorCopy = color; // Need a non-const copy
    
    return std::make_unique<Argb>(bodyId, b2LocalPosition, b2Size, colorCopy);
}

std::unique_ptr<IDipSwitch> Box2DPhysicsFactory::createDipSwitch(size_t numSwitches) {
    return std::make_unique<Box2DDipSwitch>(numSwitches);
}

std::unique_ptr<IDistanceSensor> Box2DPhysicsFactory::createDistanceSensor(
    const types::Vec2& localPosition, float angle) {
    // Convert angle to direction vector
    types::Vec2 direction = {
        std::cos(angle),
        std::sin(angle)
    };
    
    // Use a reasonable default max distance (1 meter)
    const float maxDistance = 1.0f;
    
    return std::make_unique<Box2DDistanceSensor>(worldId, bodyId, localPosition, direction, maxDistance);
}

std::unique_ptr<IMotor> Box2DPhysicsFactory::createMotor(
    const types::Vec2& localPosition, bool leftWheel,
    float angle, float resistance, float ke, float kt, float maxVoltage) {
    return std::make_unique<Box2DMotor>(
        bodyId, localPosition, leftWheel, angle,
        resistance != 0.0f ? resistance : MOTOR_RESISTANCE,
        ke != 0.0f ? ke : MOTOR_KE,
        kt != 0.0f ? kt : MOTOR_KT,
        maxVoltage != 0.0f ? maxVoltage : MOTOR_MAX_VOLTAGE
    );
}

} // namespace micrasverse::physics 
