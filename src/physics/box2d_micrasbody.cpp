#include "physics/box2d_micrasbody.hpp"
#include "physics/box2d_dipswitch.hpp"
#include "physics/box2d_distance_sensor.hpp"
#include "physics/box2d_motor.hpp"
#include "physics/box2d_button.hpp"
#include "physics/box2d_led.hpp"
#include "physics/argb.hpp"
#include "physics/box2d_rectanglebody.hpp"
#include "physics/box2d_physics_factory.hpp"
#include "constants.hpp"
#include "micrasverse_core/types.hpp"
#include "io/keyboard.hpp"
#include "physics/physics_types.hpp"

#include <cstdint>
#include <vector>
#include <cmath>
#include <iostream>

// Helper struct for sensor configuration
struct SensorConfig {
    float angle;
    b2Vec2 local_position;
};

namespace micrasverse::physics {
// Constructor
Box2DMicrasBody::Box2DMicrasBody(b2WorldId worldId, b2Vec2 position, b2Vec2 size, b2BodyType type, float density, float friction, float restitution) 
    : size(size)
    , rectBody(std::make_unique<RectangleBody>(worldId, position, size, type, density, friction, restitution))
{
    bodyId = rectBody->getBodyId();
    
    // Initialize distance sensors
    SensorConfig sensorConfigs[4] = {
        {B2_PI / 2.0f,        {-0.028f, 0.045f + MICRAS_CENTER_OF_MASS_OFFSET}},
        {5.0f * B2_PI / 6.0f, {-0.009f, 0.055f + MICRAS_CENTER_OF_MASS_OFFSET}},
        {B2_PI / 6.0f,        { 0.009f, 0.055f + MICRAS_CENTER_OF_MASS_OFFSET}},
        {B2_PI / 2.0f,        { 0.028f, 0.045f + MICRAS_CENTER_OF_MASS_OFFSET}}
    };
    
    for (size_t i = 0; i < 4; i++) {
        const auto& config = sensorConfigs[i];
        b2Vec2 direction = {std::cos(config.angle), std::sin(config.angle)};
        
        // Convert b2Vec2 to types::Vec2 for the constructor
        micrasverse::types::Vec2 localPos = {config.local_position.x, config.local_position.y};
        micrasverse::types::Vec2 directionVec = {direction.x, direction.y};
        
        distanceSensors.push_back(std::make_unique<Box2DDistanceSensor>(
            worldId,
            bodyId,
            localPos,
            directionVec,
            MAZE_FLOOR_WIDTH // max distance
        ));
    }

    // Initialize motors with proper wheel positions
    leftMotor = std::make_unique<Box2DMotor>(
        bodyId,
        micrasverse::types::Vec2{-MICRAS_HALFWIDTH, 0.0f},
        true  // isLeftWheel
    );
    
    rightMotor = std::make_unique<Box2DMotor>(
        bodyId,
        micrasverse::types::Vec2{MICRAS_HALFWIDTH, 0.0f},
        false  // isLeftWheel
    );

    // Initialize button
    button = std::make_unique<Box2DButton>(
        bodyId,
        micrasverse::types::Vec2{0.0f, MICRAS_HALFHEIGHT - 0.01f},  // position
        0.01f  // radius
    );

    // Initialize DIP switches
    dipSwitch = std::make_unique<Box2DDipSwitch>(4);

    // Initialize LED
    led = std::make_unique<Box2DLED>(
        bodyId,
        micrasverse::types::Vec2{0.0f, MICRAS_HALFHEIGHT - 0.01f},  // position
        micrasverse::types::Vec2{0.01f, 0.01f},  // size
        micrasverse::types::Color{255, 255, 255}  // color
    );

    // Initialize ARGB - fix constructor parameters
    micrasverse::types::Color defaultColor = {255, 255, 255};
    
    argbs.push_back(std::make_unique<Argb>(
        bodyId,
        b2Vec2{-0.01f, MICRAS_HALFHEIGHT/2.0f},  // position
        b2Vec2{0.0075f, 0.0075f},  // size
        defaultColor
    ));
    
    argbs.push_back(std::make_unique<Argb>(
        bodyId,
        b2Vec2{0.01f, MICRAS_HALFHEIGHT/2.0f},  // position
        b2Vec2{0.0075f, 0.0075f},  // size
        defaultColor
    ));
}

void Box2DMicrasBody::update(float deltaTime) {
    // Calculate acceleration as change in velocity over time
    b2Vec2 currentVelocity = b2Body_GetLinearVelocity(bodyId);
    this->acceleration = (currentVelocity - this->linearVelocity) * (1.0f/deltaTime);
    
    // Store current velocity for next calculation
    this->linearVelocity = currentVelocity;

    this->linearSpeed = b2Length(currentVelocity);

    // Update physical components
    updateFriction();
    
    // Update sensors
    for (auto& sensor : distanceSensors) {
        sensor->update();
    }

    // Update motors
    leftMotor->update(deltaTime);
    rightMotor->update(deltaTime);

    // Update LEDs
    led->update(deltaTime);

    // Update ARGBs
    for (auto& argb : argbs) {
        argb->update();
    }
}

void Box2DMicrasBody::processInput(float deltaTime) {
    // Process manual input
}

b2Vec2 Box2DMicrasBody::getPosition() const {
    b2Transform transform = b2Body_GetTransform(getBodyId());
    return transform.p;
}

float Box2DMicrasBody::getAngle() const {
    b2Transform transform = b2Body_GetTransform(getBodyId());
    float rotation = b2Rot_GetAngle(transform.q) ;
    
    return rotation;
}

void Box2DMicrasBody::updateFriction() {
    b2BodyId bodyId = getBodyId();
    b2Vec2 lateralVelocity = getLateralVelocity();
    float mass = b2Body_GetMass(bodyId);
    b2Vec2 position = b2Body_GetWorldCenterOfMass(bodyId);
    b2Vec2 impulse = mass * -lateralVelocity;
    b2Body_ApplyLinearImpulse(bodyId, impulse, position, true);
}

b2Vec2 Box2DMicrasBody::getLateralVelocity() const {
    b2BodyId bodyId = getBodyId();
    b2Vec2 linearVelocity = b2Body_GetLinearVelocity(bodyId);
    b2Transform transform = b2Body_GetTransform(bodyId);
    float angle = b2Rot_GetAngle(transform.q);
    b2Rot rotation = b2MakeRot(angle);
    b2Vec2 lateralNormal = b2RotateVector(rotation, b2Vec2{1.0f, 0.0f});
    return b2Dot(lateralNormal, linearVelocity) * lateralNormal;
}

// Explicit destructor definition
Box2DMicrasBody::~Box2DMicrasBody() {
    // Explicitly destroy components in reverse order of creation
    argbs.clear();
    led.reset();
    dipSwitch.reset();
    button.reset();
    rightMotor.reset();
    leftMotor.reset();
    distanceSensors.clear();
    rectBody.reset();
}

} // namespace micrasverse::physics
