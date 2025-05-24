#ifndef MICRASVERSE_PHYSICS_BOX2D_DISTANCE_SENSOR_HPP
#define MICRASVERSE_PHYSICS_BOX2D_DISTANCE_SENSOR_HPP

#include <array>

#include "box2d/box2d.h"
#include "micrasverse_core/types.hpp"

namespace micrasverse::physics {

class Box2DDistanceSensor {
public:
    Box2DDistanceSensor(b2WorldId worldId, b2BodyId bodyId, const micrasverse::types::Vec2& localPosition, float angle, float maxDistance);

    micrasverse::types::Vec2 getLocalPosition() const;

    micrasverse::types::Vec2 getDirection() const;

    void setLocalPosition(const micrasverse::types::Vec2& localPosition);

    void setDirection(const micrasverse::types::Vec2& direction);

    micrasverse::types::Vec2 getRayDirection() const;

    void update();

    float getReading() const;

    micrasverse::types::Vec2 getPosition() const;

    float getReadingVisual() const { return this->visualReading; }

    b2Vec2 getVisualMidPoint() const { return this->visualMidPoint; }

    // private:
    void performRayCast();

    b2WorldId             worldId;
    b2BodyId              bodyId;
    b2Vec2                localPosition;
    b2Vec2                localDirection;
    std::array<b2Vec2, 4> rayDirections;
    std::array<float, 4>  sensorWeights;
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
