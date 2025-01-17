#include "physics/distancesensor.hpp"
#include "core/constants.hpp"

#include "glm/glm.hpp"

#include <iostream>

namespace micrasverse::physics {

DistanceSensor::DistanceSensor(const b2BodyId bodyId, const b2Vec2 localPosition, const float angle){
    this->bodyId = bodyId;
    this->localPosition = localPosition;
    this->angle = angle;
    this->localDirection = {cos(angle), sin(angle)};

    // Create renderable object
    this->rayRender = render::Rectangle(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(237.0f, 47.0f, 50.0f) / 255.0f);
    this->rayRender.init();
}

float DistanceSensor::getReading() {
    const b2Vec2 origin = b2Body_GetWorldPoint(this->bodyId, this->localPosition);
    const b2Vec2 worldDirection = b2Body_GetWorldVector(this->bodyId, this->localDirection);
    const b2Vec2 translation = MAZE_FLOOR_WIDTH * worldDirection;
    const b2QueryFilter filter = b2DefaultQueryFilter();
    const b2RayResult output = b2World_CastRayClosest(b2Body_GetWorld(this->bodyId), origin, translation, filter);

    this->intersectionPoint = origin + output.fraction * translation;
    
    this->reading = b2Length(intersectionPoint - origin);

    this->rayRender.setPose(
        glm::vec3(origin.x + (intersectionPoint.x - origin.x)*0.5f, origin.y + (intersectionPoint.y - origin.y) *0.5f, 0.01f),
        b2Rot{ .c = -worldDirection.y, .s = worldDirection.x }
    );
    
    this->rayRender.setSize(glm::vec3(0.002f, this->reading, 0.01f));
    
    return this->reading;
}

b2Vec2 DistanceSensor::getLocalPosition(){
    return this->localPosition;
}

b2Vec2 DistanceSensor::getDirection(){
    return this->localDirection;
}

float DistanceSensor::getAngle(){ 
    return this->angle;
}

void DistanceSensor::setLocalPosition(const b2Vec2 localPosition){ 
    this->localPosition = localPosition;
}

void DistanceSensor::setDirection(const b2Vec2 direction){ 
    this->localDirection = direction;
}

void DistanceSensor::setAngle(const float angle){ 
    this->angle = angle;
}

} // namespace micrasverse::physics
