#ifndef MICRAS_PROXY_LED_HPP
#define MICRAS_PROXY_LED_HPP

#include <cstdint>
#include "box2d/box2d.h"

namespace micras::proxy {

/**
 * @brief Class for controlling an LED in the simulation.
 */
class Led {
public:
    /**
     * @brief Configuration struct for LED.
     */
    struct Config {
        b2BodyId bodyId;
        bool initial_state;
        uint8_t red;
        uint8_t green;
        uint8_t blue;
    };

    /**
     * @brief Construct a new Led object.
     *
     * @param config Configuration for the LED.
     */
    explicit Led(const Config& config);

    /**
     * @brief Turn the LED on.
     */
    void turn_on();

    /**
     * @brief Turn the LED off.
     */
    void turn_off();

    /**
     * @brief Toggle the LED.
     */
    void toggle();

    /**
     * @brief Set the state of the LED.
     * 
     * @param state True to turn on, false to turn off.
     */
    void setState(bool state);

    /**
     * @brief Get the current state of the LED.
     * 
     * @return True if the LED is on, false otherwise.
     */
    bool is_on() const;

    /**
     * @brief Get the red component of the LED color.
     * 
     * @return Red component (0-255).
     */
    uint8_t get_red() const;

    /**
     * @brief Get the green component of the LED color.
     * 
     * @return Green component (0-255).
     */
    uint8_t get_green() const;

    /**
     * @brief Get the blue component of the LED color.
     * 
     * @return Blue component (0-255).
     */
    uint8_t get_blue() const;

private:
    b2BodyId bodyId;
    bool state{false};
    uint8_t red{255};
    uint8_t green{255};
    uint8_t blue{255};
};

}  // namespace micras::proxy

#endif  // MICRAS_PROXY_LED_HPP 
