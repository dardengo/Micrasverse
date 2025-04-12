#ifndef DISTANCE_SENSOR_HPP
#define DISTANCE_SENSOR_HPP

#include "physics/i_distance_sensor.hpp"
#include "box2d/box2d.h"

namespace micrasverse::physics {

class DistanceSensor : public IDistanceSensor {
public:
    // Public properties accessed directly by other classes
    float reading;
    b2Vec2 rayMidPoint;
    b2Rot rayDirection;

    DistanceSensor(const b2BodyId bodyId, const b2Vec2 localPosition, const float angle);

    // Implement IDistanceSensor interface
    float update() override;
    float getReading() const override;
    core::Vec2 getLocalPosition() const override;
    core::Vec2 getDirection() const override;
    float getAngle() const override;
    void setLocalPosition(const core::Vec2& localPosition) override;
    void setDirection(const core::Vec2& direction) override;
    void setAngle(float angle) override;
    core::Vec2 getRayMidPoint() const override;
    core::Rotation getRayDirection() const override;

private:
    b2Vec2 localPosition;
    b2Vec2 localDirection;
    b2Vec2 intersectionPoint;
    float angle;
    b2BodyId bodyId;
};
    
}

#endif // DISTANCE_SENSOR_HPP
