#ifndef MICRASVERSE_PHYSICS_BOX2D_DISTANCE_SENSOR_HPP
#define MICRASVERSE_PHYSICS_BOX2D_DISTANCE_SENSOR_HPP

#include "box2d/box2d.h"
#include "micrasverse_core/types.hpp"
#include "physics/i_distance_sensor.hpp"

namespace micrasverse::physics {

class Box2DDistanceSensor : public IDistanceSensor {
public:

    Box2DDistanceSensor(b2WorldId worldId, b2BodyId bodyId, const micrasverse::types::Vec2& localPosition, const micrasverse::types::Vec2& direction, float maxDistance);

    micrasverse::types::Vec2 getLocalPosition() const override;

    micrasverse::types::Vec2 getDirection() const override;

    void setLocalPosition(const micrasverse::types::Vec2& localPosition) override;

    void setDirection(const micrasverse::types::Vec2& direction) override;

    micrasverse::types::Vec2 getRayMidPoint() const override { return {rayMidPoint.x, rayMidPoint.y}; }

    micrasverse::types::Vec2 getRayDirection() const override;

    void update() override;

    float getReading() const override;

    float getAngle() const override;

    void setAngle(float angle) override;

    SensorType getType() const override { return SensorType::DISTANCE; }
    micrasverse::types::Vec2 getPosition() const override;
    float getReadingVisual() const override { return reading; }

private:
    void performRayCast();

    b2WorldId worldId;
    b2BodyId bodyId;
    b2Vec2 localPosition;
    b2Vec2 localDirection;
    float maxDistance;
    float reading;
    float angle;
    b2Vec2 rayMidPoint;
    b2Vec2 rayDirection;
    b2Vec2 intersectionPoint;
};

}  // namespace micrasverse::physics

#endif  // MICRASVERSE_PHYSICS_BOX2D_DISTANCE_SENSOR_HPP 
