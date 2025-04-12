#ifndef I_SENSOR_HPP
#define I_SENSOR_HPP

#include "core/types.hpp"

namespace micrasverse::physics {

enum class SensorType {
    DISTANCE,
    LIGHT,
    TOUCH,
    IMU,
    ENCODER
};

class ISensor {
public:
    virtual ~ISensor() = default;
    
    // Sensor basics
    virtual SensorType getType() const = 0;
    virtual core::Vec2 getPosition() const = 0;
    virtual core::Vec2 getDirection() const = 0;
    
    // Reading
    virtual float getReading() const = 0;
    virtual void update() = 0;
    
    // For visualization
    virtual core::Vec2 getRayMidPoint() const = 0;
    virtual core::Vec2 getRayDirection() const = 0;
    virtual float getReadingVisual() const = 0; // The value for display purposes
};

} // namespace micrasverse::physics

#endif // I_SENSOR_HPP 
