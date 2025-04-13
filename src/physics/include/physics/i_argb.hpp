#ifndef MICRASVERSE_PHYSICS_I_ARGB_HPP
#define MICRASVERSE_PHYSICS_I_ARGB_HPP

#include "micrasverse_core/types.hpp"
#include "micras/core/types.hpp"
#include "physics/i_actuator.hpp"
#include <array>

namespace micrasverse::physics {

/**
 * @brief Interface for ARGB LEDs.
 */
class IArgb : public IActuator {
public:
    virtual ~IArgb() = default;

    // Update the position of the LED
    virtual void update() = 0;

    /**
     * @brief Set the color of the LED.
     *
     * @param color The color to set.
     */
    virtual void setColor(const micrasverse::types::Color& color) = 0;

    /**
     * @brief Get the color of the LED.
     *
     * @return The color of the LED.
     */
    virtual micrasverse::types::Color getColor() const = 0;

    /**
     * @brief Turn on the LED.
     */
    virtual void turnOn() = 0;

    /**
     * @brief Turn on the LED with a specific color.
     *
     * @param color The color to set.
     */
    virtual void turnOn(const micrasverse::types::Color& color) = 0;

    /**
     * @brief Turn off the LED.
     */
    virtual void turnOff() = 0;

    /**
     * @brief Toggle the LED state.
     */
    virtual void toggle() = 0;

    /**
     * @brief Check if the LED is on.
     *
     * @return True if the LED is on, false otherwise.
     */
    virtual bool isOn() const = 0;

    /**
     * @brief Get the world position of the LED.
     *
     * @return The world position of the LED.
     */
    virtual micrasverse::types::Vec2 getWorldPosition() const = 0;

    /**
     * @brief Get the size of the LED.
     *
     * @return The size of the LED.
     */
    virtual micrasverse::types::Vec2 getSize() const = 0;

    /**
     * @brief Attach an ARGB LED to a body.
     *
     * @param localPosition The local position of the LED.
     * @param size The size of the LED.
     * @param color The color of the LED.
     */
    virtual void attachArgb(micrasverse::types::Vec2 localPosition, micrasverse::types::Vec2 size, micrasverse::types::Color color) = 0;

    /**
     * @brief Set the color of a specific LED in the strip.
     *
     * @param color The color to set.
     * @param index The index of the LED.
     */
    virtual void set_color(const micrasverse::types::Color& color, uint8_t index) = 0;

    /**
     * @brief Set the colors of all LEDs in the strip.
     *
     * @param colors The colors to set.
     */
    virtual void set_colors(const std::array<micrasverse::types::Color, 8>& colors) = 0;

    /**
     * @brief Encode a color for a specific LED.
     *
     * @param color The color to encode.
     * @param index The index of the LED.
     */
    virtual void encode_color(const micrasverse::types::Color& color, uint8_t index) = 0;
    
    // Override the getType method from IActuator
    virtual ActuatorType getType() const override { return ActuatorType::LED; }
};

} // namespace micrasverse::physics

#endif // MICRASVERSE_PHYSICS_I_ARGB_HPP 
