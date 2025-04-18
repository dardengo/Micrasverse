#ifndef MICRAS_PROXY_FAN_HPP
#define MICRAS_PROXY_FAN_HPP

#include <cstdint>
#include "box2d/box2d.h"
#include "micras/proxy/stopwatch.hpp"
#include <memory>

namespace micras::proxy {

/**
 * @brief Class for controlling the fan driver in the simulation.
 */
class Fan {
public:
    /**
     * @brief Enum for rotation direction.
     */
    enum RotationDirection : uint8_t {
        FORWARD,
        BACKWARDS
    };

    /**
     * @brief Configuration struct for the fan.
     */
    struct Config {
        b2BodyId bodyId;
        float max_acceleration;  // Maximum acceleration in speed units per second
        float max_speed;         // Maximum speed in speed units
    };

    /**
     * @brief Construct a new fan object.
     *
     * @param config Configuration for the fan driver.
     */
    explicit Fan(const Config& config);

    /**
     * @brief Enable the fan.
     */
    void enable();

    /**
     * @brief Disable the fan.
     */
    void disable();

    /**
     * @brief Set the speed of the fans.
     *
     * @param speed Speed percentage of the fan (-100 to 100).
     */
    void set_speed(float speed);

    /**
     * @brief Update the speed of the fan.
     * 
     * @return Current speed of the fan.
     */
    float update();

    /**
     * @brief Stop the fan.
     */
    void stop();

    /**
     * @brief Get the current speed of the fan.
     * 
     * @return Current speed of the fan.
     */
    float get_speed() const;

    /**
     * @brief Set the rotation direction of the fan.
     *
     * @param direction Rotation direction.
     */
    void set_direction(RotationDirection direction);

private:
    b2BodyId bodyId;
    float max_acceleration;
    float max_speed;
    float current_speed = 0.0f;
    float target_speed = 0.0f;
    bool enabled = false;
    float last_update_time{0.0f};
    RotationDirection current_direction = RotationDirection::FORWARD;
    std::unique_ptr<Stopwatch> stopwatch;
};

}  // namespace micras::proxy

#endif  // MICRAS_PROXY_FAN_HPP 
