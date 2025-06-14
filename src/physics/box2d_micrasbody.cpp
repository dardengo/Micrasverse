#include "physics/box2d_micrasbody.hpp"
#include "physics/box2d_distance_sensor.hpp"
#include "physics/box2d_motor.hpp"
#include "physics/box2d_rectanglebody.hpp"
#include "constants.hpp"
#include "micrasverse_core/types.hpp"
#include "io/keyboard.hpp"

#include <cstdint>
#include <vector>
#include <cmath>
#include <iostream>

// Helper struct for sensor configuration
struct SensorConfig {
    float  angle;
    b2Vec2 local_position;
};

namespace micrasverse::physics {
// Constructor
Box2DMicrasBody::Box2DMicrasBody(b2WorldId worldId, b2Vec2 position, b2Vec2 size, b2BodyType type, float density, float friction, float restitution) :
    size(size), rectBody(std::make_unique<RectangleBody>(worldId, position, size, type, density, friction, restitution)) {
    bodyId = rectBody->getBodyId();

    // Initialize distance sensors
    SensorConfig sensorConfigs[4] = {
        {B2_PI / 2.0f, {-MICRAS_HALFWIDTH, MICRAS_HALFHEIGHT}},
        {5.0f * B2_PI / 6.0f, {-MICRAS_HALFWIDTH / 2, MICRAS_HALFHEIGHT}},
        {B2_PI / 6.0f, {MICRAS_HALFWIDTH / 2, MICRAS_HALFHEIGHT}},
        {B2_PI / 2.0f, {MICRAS_HALFWIDTH, MICRAS_HALFHEIGHT}}
    };

    for (size_t i = 0; i < 4; i++) {
        const auto& config = sensorConfigs[i];

        // Convert b2Vec2 to types::Vec2 for the constructor
        micrasverse::types::Vec2 localPos = {config.local_position.x, config.local_position.y};

        distanceSensors.push_back(std::make_unique<Box2DDistanceSensor>(
            worldId, bodyId, localPos, config.angle,
            MAZE_FLOOR_WIDTH  // max distance
        ));
    }

    // Initialize motors with proper wheel positions
    leftMotor = std::make_unique<Box2DMotor>(
        bodyId, micrasverse::types::Vec2{-MICRAS_HALFWIDTH, 0.0f},
        true  // isLeftWheel
    );

    rightMotor = std::make_unique<Box2DMotor>(
        bodyId, micrasverse::types::Vec2{MICRAS_HALFWIDTH, 0.0f},
        false  // isLeftWheel
    );
}

void Box2DMicrasBody::update(float deltaTime) {
    // Calculate acceleration as change in velocity over time
    b2Vec2 currentVelocity = b2Body_GetLinearVelocity(bodyId);
    this->acceleration = (currentVelocity - this->linearVelocity) * (1.0f / deltaTime);

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
    float       rotation = b2Rot_GetAngle(transform.q);

    return rotation;
}

void Box2DMicrasBody::updateFriction() {
    b2BodyId bodyId = getBodyId();
    b2Vec2   lateralVelocity = getLateralVelocity();
    float    mass = b2Body_GetMass(bodyId);
    b2Vec2   position = b2Body_GetWorldCenterOfMass(bodyId);
    b2Vec2   impulse = mass * -lateralVelocity;
    b2Body_ApplyLinearImpulse(bodyId, impulse, position, true);
}

b2Vec2 Box2DMicrasBody::getLateralVelocity() const {
    b2BodyId    bodyId = getBodyId();
    b2Vec2      linearVelocity = b2Body_GetLinearVelocity(bodyId);
    b2Transform transform = b2Body_GetTransform(bodyId);
    float       angle = b2Rot_GetAngle(transform.q);
    b2Rot       rotation = b2MakeRot(angle);
    b2Vec2      lateralNormal = b2RotateVector(rotation, b2Vec2{1.0f, 0.0f});
    return b2Dot(lateralNormal, linearVelocity) * lateralNormal;
}

// Explicit destructor definition
Box2DMicrasBody::~Box2DMicrasBody() {
    // Explicitly destroy components in reverse order of creation
    rightMotor.reset();
    leftMotor.reset();
    distanceSensors.clear();
    rectBody.reset();
}

}  // namespace micrasverse::physics
