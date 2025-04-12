#include "physics/box2d_distance_sensor.hpp"
#include <cmath>

namespace micrasverse::physics {

Box2DDistanceSensor::Box2DDistanceSensor(b2WorldId worldId, b2BodyId bodyId, const core::Vec2& localPosition, const core::Vec2& direction, float maxDistance)
    : worldId(worldId), bodyId(bodyId), 
      localPosition{localPosition.x, localPosition.y}, 
      localDirection{direction.x, direction.y}, 
      maxDistance(maxDistance), reading(0.0f),
      rayMidPoint{0.0f, 0.0f}, rayDirection{0.0f, 0.0f} {
    // Initialize the sensor
    update();
}

core::Vec2 Box2DDistanceSensor::getPosition() const {
    // Convert local position to world position
    b2Vec2 worldPos = b2Body_GetWorldPoint(bodyId, localPosition);
    return {worldPos.x, worldPos.y};
}

core::Vec2 Box2DDistanceSensor::getDirection() const {
    // Convert local direction to world direction
    b2Vec2 worldDir = b2Body_GetWorldVector(bodyId, localDirection);
    return {worldDir.x, worldDir.y};
}

float Box2DDistanceSensor::getReading() const {
    return reading;
}

void Box2DDistanceSensor::update() {
    performRayCast();
}

void Box2DDistanceSensor::performRayCast() {
    // Get world position and direction
    b2Vec2 origin = b2Body_GetWorldPoint(bodyId, localPosition);
    b2Vec2 worldDir = b2Body_GetWorldVector(bodyId, localDirection);
    
    // Normalize direction
    float length = std::sqrt(worldDir.x * worldDir.x + worldDir.y * worldDir.y);
    if (length > 0.0f) {
        worldDir.x /= length;
        worldDir.y /= length;
    }
    
    // Calculate ray end point (using translation)
    b2Vec2 translation = {worldDir.x * maxDistance, worldDir.y * maxDistance};
    
    // Setup ray cast query
    const b2QueryFilter filter = b2DefaultQueryFilter();
    
    // Perform ray cast
    const b2RayResult result = b2World_CastRayClosest(worldId, origin, translation, filter);
    
    // Update reading based on ray cast results
    // Check if we hit something (fraction < 1.0)
    b2Vec2 intersectionPoint;
    if (result.fraction < 1.0f) {
        // Calculate actual hit point
        reading = result.fraction * maxDistance;
        intersectionPoint = {
            origin.x + translation.x * result.fraction,
            origin.y + translation.y * result.fraction
        };
    } else {
        // No hit - use max distance
        reading = maxDistance;
        intersectionPoint = {
            origin.x + translation.x,  // Full length ray
            origin.y + translation.y
        };
    }
    
    // Always update visualization data to show the ray
    rayMidPoint = {
        origin.x + (intersectionPoint.x - origin.x) * 0.5f,
        origin.y + (intersectionPoint.y - origin.y) * 0.5f
    };
    
    // Store direction for visualization
    rayDirection = worldDir;
}

} // namespace micrasverse::physics 
