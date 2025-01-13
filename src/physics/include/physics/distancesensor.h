#ifndef DISTANCE_SENSOR_H
#define DISTANCE_SENSOR_H

#include "models/rectangle.h"

#include "box2d/box2d.h"

namespace micrasverse::physics {

class DistanceSensor {
public:
    float reading;

    render::Rectangle rayRender;

    DistanceSensor(const b2BodyId bodyId, const b2Vec2 localPosition, const float angle);

    float getReading();

    b2Vec2 getLocalPosition();

    b2Vec2 getDirection();
    
    float getAngle();

    void setLocalPosition(const b2Vec2 localPosition);

    void setDirection(const b2Vec2 direction);

    void setAngle(const float angle);

private:
    b2Vec2 localPosition;
    b2Vec2 localDirection;
    b2Vec2 intersectionPoint;
    float angle;
    b2BodyId bodyId;
};
    
}

#endif // DISTANCE_SENSOR_H
