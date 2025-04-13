#ifndef MICRASVERSE_PHYSICS_I_DISTANCE_SENSOR_HPP
#define MICRASVERSE_PHYSICS_I_DISTANCE_SENSOR_HPP

#include "micrasverse_core/types.hpp"
#include "physics/i_sensor.hpp"

namespace micrasverse::physics {

/**
 * @brief Interface for distance sensors.
 */
class IDistanceSensor : public ISensor {
public:
    virtual ~IDistanceSensor() = default;

    /**
     * @brief Get the local position of the sensor.
     *
     * @return The local position of the sensor.
     */
    virtual micrasverse::types::Vec2 getLocalPosition() const = 0;

    /**
     * @brief Get the direction of the sensor.
     *
     * @return The direction of the sensor.
     */
    virtual micrasverse::types::Vec2 getDirection() const = 0;

    /**
     * @brief Set the local position of the sensor.
     *
     * @param localPosition The new local position of the sensor.
     */
    virtual void setLocalPosition(const micrasverse::types::Vec2& localPosition) = 0;

    /**
     * @brief Set the direction of the sensor.
     *
     * @param direction The new direction of the sensor.
     */
    virtual void setDirection(const micrasverse::types::Vec2& direction) = 0;

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

    /**
     * @brief Update the sensor reading.
     */
    virtual void update() = 0;

    /**
     * @brief Get the current reading of the sensor.
     *
     * @return The current reading.
     */
    virtual float getReading() const = 0;

    /**
     * @brief Get the angle of the sensor.
     *
     * @return The angle of the sensor.
     */
    virtual float getAngle() const = 0;

    /**
     * @brief Set the angle of the sensor.
     *
     * @param angle The new angle of the sensor.
     */
    virtual void setAngle(float angle) = 0;
};

} // namespace micrasverse::physics

#endif // MICRASVERSE_PHYSICS_I_DISTANCE_SENSOR_HPP 
