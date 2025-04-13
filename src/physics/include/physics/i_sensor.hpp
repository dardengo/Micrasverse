#ifndef MICRASVERSE_PHYSICS_I_SENSOR_HPP
#define MICRASVERSE_PHYSICS_I_SENSOR_HPP

#include "micrasverse_core/types.hpp"
#include "micras/core/types.hpp"

namespace micrasverse::physics {

enum class SensorType {
    DISTANCE,
    LIGHT,
    TOUCH,
    IMU,
    ENCODER
};

/**
 * @brief Interface for sensors.
 */
class ISensor {
public:
    virtual ~ISensor() = default;
    
    // Sensor basics
    virtual SensorType getType() const = 0;

    /**
     * @brief Get the position of the sensor.
     *
     * @return The position of the sensor.
     */
    virtual micrasverse::types::Vec2 getPosition() const = 0;

    /**
     * @brief Get the direction of the sensor.
     *
     * @return The direction of the sensor.
     */
    virtual micrasverse::types::Vec2 getDirection() const = 0;
    
    // Reading
    virtual float getReading() const = 0;
    virtual void update() = 0;
    
    // For visualization
    /**
     * @brief Get the midpoint of the ray.
     *
     * @return The midpoint of the ray.
     */
    virtual micrasverse::types::Vec2 getRayMidPoint() const = 0;

    /**
     * @brief Get the direction of the ray.
     *
     * @return The direction of the ray.
     */
    virtual micrasverse::types::Vec2 getRayDirection() const = 0;
    virtual float getReadingVisual() const = 0; // The value for display purposes
};

} // namespace micrasverse::physics

#endif // MICRASVERSE_PHYSICS_I_SENSOR_HPP 
