#ifndef I_DISTANCE_SENSOR_HPP
#define I_DISTANCE_SENSOR_HPP

#include "core/types.hpp"

namespace micrasverse::physics {

class IDistanceSensor {
public:
    virtual ~IDistanceSensor() = default;

    // Update method that performs the actual calculation
    virtual float update() = 0;
    
    // Method that simply returns the last cached reading
    virtual float getReading() const = 0;

    // Get/set position and direction
    virtual core::Vec2 getLocalPosition() const = 0;
    virtual core::Vec2 getDirection() const = 0;
    virtual float getAngle() const = 0;
    virtual void setLocalPosition(const core::Vec2& localPosition) = 0;
    virtual void setDirection(const core::Vec2& direction) = 0;
    virtual void setAngle(float angle) = 0;

    // Visualization data accessors
    virtual core::Vec2 getRayMidPoint() const = 0;
    virtual core::Rotation getRayDirection() const = 0;
};
    
} // namespace micrasverse::physics

#endif // I_DISTANCE_SENSOR_HPP 
