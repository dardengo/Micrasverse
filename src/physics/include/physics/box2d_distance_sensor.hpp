#ifndef MICRASVERSE_PHYSICS_BOX2D_DISTANCE_SENSOR_HPP
#define MICRASVERSE_PHYSICS_BOX2D_DISTANCE_SENSOR_HPP

#include "box2d/box2d.h"
#include "micrasverse_core/types.hpp"
#include "physics/i_distance_sensor.hpp"

namespace micrasverse::physics {

/**
 * @brief Box2D implementation of a distance sensor.
 */
class Box2DDistanceSensor : public IDistanceSensor {
public:
    /**
     * @brief Construct a new Box2DDistanceSensor object.
     *
     * @param worldId The Box2D world ID.
     * @param bodyId The Box2D body ID.
     * @param localPosition The local position of the sensor.
     * @param direction The direction of the sensor.
     * @param maxDistance The maximum distance the sensor can detect.
     */
    Box2DDistanceSensor(b2WorldId worldId, b2BodyId bodyId, const micrasverse::types::Vec2& localPosition, const micrasverse::types::Vec2& direction, float maxDistance);

    /**
     * @brief Get the local position of the sensor.
     *
     * @return The local position of the sensor.
     */
    micrasverse::types::Vec2 getLocalPosition() const override;

    /**
     * @brief Get the direction of the sensor.
     *
     * @return The direction of the sensor.
     */
    micrasverse::types::Vec2 getDirection() const override;

    /**
     * @brief Set the local position of the sensor.
     *
     * @param localPosition The new local position of the sensor.
     */
    void setLocalPosition(const micrasverse::types::Vec2& localPosition) override;

    /**
     * @brief Set the direction of the sensor.
     *
     * @param direction The new direction of the sensor.
     */
    void setDirection(const micrasverse::types::Vec2& direction) override;

    /**
     * @brief Get the midpoint of the ray.
     *
     * @return The midpoint of the ray.
     */
    micrasverse::types::Vec2 getRayMidPoint() const override { return {rayMidPoint.x, rayMidPoint.y}; }

    /**
     * @brief Get the direction of the ray.
     *
     * @return The direction of the ray.
     */
    micrasverse::types::Vec2 getRayDirection() const override;

    /**
     * @brief Update the sensor reading.
     */
    void update() override;

    /**
     * @brief Get the current reading of the sensor.
     *
     * @return The current reading.
     */
    float getReading() const override;

    /**
     * @brief Get the angle of the sensor.
     *
     * @return The angle of the sensor.
     */
    float getAngle() const override;

    /**
     * @brief Set the angle of the sensor.
     *
     * @param angle The new angle of the sensor.
     */
    void setAngle(float angle) override;

    // ISensor interface implementation
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
