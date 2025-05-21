#include "physics/box2d_distance_sensor.hpp"
#include "constants.hpp"
#include <cmath>

namespace micrasverse::physics {

Box2DDistanceSensor::Box2DDistanceSensor(
    b2WorldId worldId, b2BodyId bodyId, const micrasverse::types::Vec2& localPosition, float angle, float maxDistance
) :
    worldId(worldId),
    bodyId(bodyId),
    localPosition{localPosition.x, localPosition.y},
    localDirection{std::cos(angle), std::sin(angle)},
    rayDirections{{
        {std::cos(angle - B2_PI / 18.0F), std::sin(angle - B2_PI / 18.0F)},
        {std::cos(angle - B2_PI / 36.0F), std::sin(angle - B2_PI / 36.0F)},
        {std::cos(angle + B2_PI / 18.0F), std::sin(angle + B2_PI / 18.0F)},
        {std::cos(angle + B2_PI / 36.0F), std::sin(angle + B2_PI / 36.0F)},
    }},
    maxDistance(maxDistance),
    reading(0.0f),
    rayMidPoint{0.0f, 0.0f},
    rayDirection{0.0f, 0.0f} {
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

micrasverse::types::Vec2 Box2DDistanceSensor::getPosition() const {
    b2Vec2 worldPos = b2Body_GetWorldPoint(bodyId, localPosition);
    return {worldPos.x, worldPos.y};
}

void Box2DDistanceSensor::performRayCast() {
    const b2Vec2 origin = b2Body_GetWorldPoint(this->bodyId, this->localPosition);
    this->reading = 0.0F;

    for (const auto& rayDirection : this->rayDirections) {
        this->worldDirection = b2Body_GetWorldVector(this->bodyId, rayDirection);
        const b2Vec2        translation = this->maxDistance * worldDirection;
        const b2QueryFilter filter = b2DefaultQueryFilter();
        const b2RayResult   output = b2World_CastRayClosest(b2Body_GetWorld(this->bodyId), origin, translation, filter);
        this->intersectionPoint = origin + output.fraction * translation;
        this->reading += b2Length(intersectionPoint - origin);
    }

    this->worldDirection = b2Body_GetWorldVector(this->bodyId, this->localDirection);
    const b2Vec2        translation = this->maxDistance * worldDirection;
    const b2QueryFilter filter = b2DefaultQueryFilter();
    const b2RayResult   output = b2World_CastRayClosest(b2Body_GetWorld(this->bodyId), origin, translation, filter);
    this->intersectionPoint = origin + output.fraction * translation;
    this->reading += b2Length(intersectionPoint - origin);

    this->reading /= 5.0F;

    this->rayMidPoint = b2Vec2{origin.x + (intersectionPoint.x - origin.x) * 0.5f, origin.y + (intersectionPoint.y - origin.y) * 0.5f};

    this->rayDirection = {-worldDirection.y, worldDirection.x};
}

}  // namespace micrasverse::physics
