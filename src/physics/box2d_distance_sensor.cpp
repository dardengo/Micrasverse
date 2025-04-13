#include "physics/box2d_distance_sensor.hpp"
#include <cmath>

namespace micrasverse::physics {

Box2DDistanceSensor::Box2DDistanceSensor(b2WorldId worldId, b2BodyId bodyId, const micrasverse::types::Vec2& localPosition, const micrasverse::types::Vec2& direction, float maxDistance)
    : worldId(worldId), bodyId(bodyId), 
      localPosition{localPosition.x, localPosition.y}, 
      localDirection{direction.x, direction.y}, 
      maxDistance(maxDistance), reading(0.0f), angle(0.0f),
      rayMidPoint{0.0f, 0.0f}, rayDirection{0.0f, 0.0f} {
    // Initialize the sensor
    update();
}

micrasverse::types::Vec2 Box2DDistanceSensor::getLocalPosition() const {
    return {localPosition.x, localPosition.y};
}

micrasverse::types::Vec2 Box2DDistanceSensor::getDirection() const {
    return {localDirection.x, localDirection.y};
}

void Box2DDistanceSensor::setLocalPosition(const micrasverse::types::Vec2& newLocalPosition) {
    localPosition.x = newLocalPosition.x;
    localPosition.y = newLocalPosition.y;
    update();
}

void Box2DDistanceSensor::setDirection(const micrasverse::types::Vec2& newDirection) {
    localDirection.x = newDirection.x;
    localDirection.y = newDirection.y;
    update();
}

micrasverse::types::Vec2 Box2DDistanceSensor::getRayDirection() const {
    return {rayDirection.x, rayDirection.y};
}

void Box2DDistanceSensor::update() {
    performRayCast();
}

float Box2DDistanceSensor::getReading() const {
    return reading;
}

float Box2DDistanceSensor::getAngle() const {
    return angle;
}

void Box2DDistanceSensor::setAngle(float newAngle) {
    angle = newAngle;
    // Update the direction based on the new angle
    localDirection.x = std::cos(angle);
    localDirection.y = std::sin(angle);
    update();
}

// ISensor interface implementation
micrasverse::types::Vec2 Box2DDistanceSensor::getPosition() const {
    // Get the world position of the sensor using Box2D's API
    b2Vec2 worldPos = b2Body_GetWorldPoint(bodyId, localPosition);
    return {worldPos.x, worldPos.y};
}

void Box2DDistanceSensor::performRayCast() {
    // Get the world position of the sensor
    b2Vec2 worldPos = b2Body_GetWorldPoint(bodyId, localPosition);
    
    // Calculate the ray direction in world space
    b2Vec2 worldDir = b2Body_GetWorldVector(bodyId, localDirection);
    
    // Store the ray midpoint and direction for visualization
    rayMidPoint = worldPos;
    rayDirection = worldDir;
    
    // In Box2D 3.1, we use b2World_CastRayClosest for ray casting
    // The direction vector should include the maxDistance
    b2Vec2 rayEnd = {worldDir.x * maxDistance, worldDir.y * maxDistance};
    b2QueryFilter filter = b2DefaultQueryFilter();
    b2RayResult result = b2World_CastRayClosest(worldId, worldPos, rayEnd, filter);
    
    if (result.hit) {
        reading = result.fraction * maxDistance;
    } else {
        reading = maxDistance;
    }
}

} // namespace micrasverse::physics 
