#ifndef MICRAS_PROXY_STOPWATCH_HPP
#define MICRAS_PROXY_STOPWATCH_HPP

#include <cstdint>
#include "box2d/box2d.h"

namespace micras::proxy {

/**
 * @brief Class to measure the time elapsed between two events in the simulation.
 */
class Stopwatch {
public:
    /**
     * @brief Stopwatch configuration struct.
     */
    struct Config {
        // No configuration needed for simulation
    };

    /**
     * @brief Construct a new Stopwatch object.
     */
    Stopwatch();

    /**
     * @brief Construct a new Stopwatch object.
     *
     * @param config Configuration for the timer.
     * @param bodyId Box2D body ID for the robot.
     */
    explicit Stopwatch(const Config& config, const b2BodyId bodyId);

    /**
     * @brief Reset the milliseconds timer counter.
     */
    void reset_ms();

    /**
     * @brief Reset the microseconds timer counter.
     */
    void reset_us();

    /**
     * @brief Get the time elapsed since the last reset.
     *
     * @return Time elapsed in milliseconds.
     */
    uint32_t elapsed_time_ms() const;

    /**
     * @brief Get the time elapsed since the last reset.
     *
     * @return Time elapsed in microseconds.
     */
    uint32_t elapsed_time_us() const;

    /**
     * @brief Sleep for a given amount of time.
     *
     * @param time Time to sleep in milliseconds.
     */
    void sleep_ms(uint32_t time);

    /**
     * @brief Sleep for a given amount of time.
     *
     * @param time Time to sleep in microseconds.
     */
    void sleep_us(uint32_t time);

private:
    b2BodyId bodyId;
    float start_time{0.0f};
};

}  // namespace micras::proxy

#endif  // MICRAS_PROXY_STOPWATCH_HPP 
