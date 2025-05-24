#ifndef MICRASVERSE_PHYSICS_BOX2D_DISTANCE_SENSOR_HPP
#define MICRASVERSE_PHYSICS_BOX2D_DISTANCE_SENSOR_HPP

#include <array>

#include "box2d/box2d.h"
#include "micrasverse_core/types.hpp"
#include "physics/i_distance_sensor.hpp"

namespace micrasverse::physics {

class Box2DDistanceSensor : public IDistanceSensor {
public:
    Box2DDistanceSensor(b2WorldId worldId, b2BodyId bodyId, const micrasverse::types::Vec2& localPosition, float angle, float maxDistance);

    micrasverse::types::Vec2 getLocalPosition() const override;

    micrasverse::types::Vec2 getDirection() const override;

    void setLocalPosition(const micrasverse::types::Vec2& localPosition) override;

    void setDirection(const micrasverse::types::Vec2& direction) override;

    micrasverse::types::Vec2 getRayDirection() const override;

    void update() override;

    float getReading() const override;

    SensorType getType() const override { return SensorType::DISTANCE; }

    micrasverse::types::Vec2 getPosition() const override;

    float getReadingVisual() const override { return this->visualReading; }

    b2Vec2 getVisualMidPoint() const { return this->visualMidPoint; }

    // private:
    void performRayCast();

    b2WorldId             worldId;
    b2BodyId              bodyId;
    b2Vec2                localPosition;
    b2Vec2                localDirection;
    std::array<b2Vec2, 4> rayDirections;
    b2Vec2                worldDirection;
    float                 maxDistance;
    float                 reading;
    float                 angle;
    b2Vec2                rayDirection;
    b2Vec2                intersectionPoint;
    float                 visualReading;
    b2Vec2                visualMidPoint;
};

}  // namespace micrasverse::physics

#endif  // MICRASVERSE_PHYSICS_BOX2D_DISTANCE_SENSOR_HPP
