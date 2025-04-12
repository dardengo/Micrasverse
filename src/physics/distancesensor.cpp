#include "physics/distancesensor.hpp"
#include "config/constants.hpp"

namespace micrasverse::physics {

DistanceSensor::DistanceSensor(const b2BodyId bodyId, const b2Vec2 localPosition, const float angle){
    this->bodyId = bodyId;
    this->localPosition = localPosition;
    this->angle = angle;
    this->localDirection = {cos(angle), sin(angle)};
    this->reading = 0.0f; // Initialize reading to 0
}

float DistanceSensor::update() {
    const b2Vec2 origin = b2Body_GetWorldPoint(this->bodyId, this->localPosition);
    const b2Vec2 worldDirection = b2Body_GetWorldVector(this->bodyId, this->localDirection);
    const b2Vec2 translation = MAZE_FLOOR_WIDTH * worldDirection;
    const b2QueryFilter filter = b2DefaultQueryFilter();
    const b2RayResult output = b2World_CastRayClosest(b2Body_GetWorld(this->bodyId), origin, translation, filter);

    this->intersectionPoint = origin + output.fraction * translation;
    
    this->reading = b2Length(intersectionPoint - origin);

    this->rayMidPoint = b2Vec2{origin.x + (intersectionPoint.x - origin.x)*0.5f, origin.y + (intersectionPoint.y - origin.y) *0.5f};

    this->rayDirection = b2Rot{ .c = -worldDirection.y, .s = worldDirection.x };

    return this->reading;
}

float DistanceSensor::getReading() const {
    return this->reading;
}

core::Vec2 DistanceSensor::getLocalPosition() const {
    return {this->localPosition.x, this->localPosition.y};
}

core::Vec2 DistanceSensor::getDirection() const {
    return {this->localDirection.x, this->localDirection.y};
}

float DistanceSensor::getAngle() const { 
    return this->angle;
}

void DistanceSensor::setLocalPosition(const core::Vec2& localPosition) { 
    this->localPosition = {localPosition.x, localPosition.y};
}

void DistanceSensor::setDirection(const core::Vec2& direction) { 
    this->localDirection = {direction.x, direction.y};
}

void DistanceSensor::setAngle(float angle) { 
    this->angle = angle;
}

core::Vec2 DistanceSensor::getRayMidPoint() const {
    return {this->rayMidPoint.x, this->rayMidPoint.y};
}

core::Rotation DistanceSensor::getRayDirection() const {
    return {this->rayDirection.c, this->rayDirection.s};
}

} // namespace micrasverse::physics
