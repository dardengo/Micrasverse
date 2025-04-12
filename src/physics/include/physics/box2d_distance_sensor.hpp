#ifndef BOX2D_DISTANCE_SENSOR_HPP
#define BOX2D_DISTANCE_SENSOR_HPP

#include "physics/i_sensor.hpp"
#include "box2d/box2d.h"

namespace micrasverse::physics {

class Box2DDistanceSensor : public ISensor {
public:
    Box2DDistanceSensor(b2WorldId worldId, b2BodyId bodyId, const core::Vec2& localPosition, const core::Vec2& direction, float maxDistance);
    virtual ~Box2DDistanceSensor() = default;

    // ISensor interface implementation
    SensorType getType() const override { return SensorType::DISTANCE; }
    core::Vec2 getPosition() const override;
    core::Vec2 getDirection() const override;
    
    float getReading() const override;
    void update() override;
    
    core::Vec2 getRayMidPoint() const override { return {rayMidPoint.x, rayMidPoint.y}; }
    core::Vec2 getRayDirection() const override { return {rayDirection.x, rayDirection.y}; }
    
    // Always return a positive reading for visualization
    float getReadingVisual() const override { 
        // This ensures the ray is always visible in the renderer, even at max distance
        return reading > 0.0f ? reading : maxDistance; 
    }

private:
    b2WorldId worldId;
    b2BodyId bodyId;
    b2Vec2 localPosition;
    b2Vec2 localDirection;
    float maxDistance;
    float reading;
    
    // For visualization
    b2Vec2 rayMidPoint;
    b2Vec2 rayDirection;
    
    void performRayCast();
};

} // namespace micrasverse::physics

#endif // BOX2D_DISTANCE_SENSOR_HPP 
