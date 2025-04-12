#ifndef I_ARGB_HPP
#define I_ARGB_HPP

#include "core/types.hpp"
#include <array>

namespace micrasverse::physics {

class IARGB {
public:
    virtual ~IARGB() = default;

    // Update the position of the LED
    virtual void update() = 0;

    // Set/Get the color of the LED
    virtual void setColor(const core::Color& color) = 0;
    virtual core::Color getColor() const = 0;

    // Control LED state
    virtual void turnOn() = 0;
    virtual void turnOn(const core::Color& color) = 0;
    virtual void turnOff() = 0;
    virtual void toggle() = 0;
    virtual bool isOn() const = 0;
    
    // Position and size accessors for rendering
    virtual core::Vec2 getWorldPosition() const = 0;
    virtual core::Vec2 getSize() const = 0;
};

} // namespace micrasverse::physics

#endif // I_ARGB_HPP 
