#ifndef MICRASVERSE_PHYSICS_ARGB_HPP
#define MICRASVERSE_PHYSICS_ARGB_HPP

#include "micrasverse_core/types.hpp"
#include "box2d/box2d.h"
#include <memory>
#include <array>

namespace micrasverse::physics {

class Argb {
public:
    // Default constructor
    Argb() = default;

    // Constructor with parameters
    Argb(b2BodyId bodyId, b2Vec2 localPosition, b2Vec2 size, const types::Color& lightColor);

    virtual ~Argb() = default;

    void         update();
    void         setColor(const types::Color& color);
    types::Color getColor() const;
    void         turnOn();
    void         turnOn(const types::Color& color);
    void         turnOff();
    void         toggle();
    bool         isOn() const;
    types::Vec2  getWorldPosition() const;
    types::Vec2  getSize() const;
    void         attachArgb(types::Vec2 localPosition, types::Vec2 size, types::Color color);
    void         set_color(const types::Color& color, uint8_t index);
    void         set_colors(const std::array<types::Color, 8>& colors);
    void         encode_color(const types::Color& color, uint8_t index);

    types::Vec2 getPosition() const;
    void        setCommand(float command);
    float       getCommand() const;
    bool        isActive() const;
    void        update(float deltaTime);

    // Additional getter methods
    const std::array<float, 3>& getLightColorArray() const { return lightColorArray; }

    const std::array<float, 3>& getBaseColorArray() const { return baseColorArray; }

    // private:
    b2BodyId             bodyId;
    b2Vec2               localPosition;
    b2Vec2               worldPosition;
    b2Vec2               size;
    bool                 isLedOn{false};
    types::Color         lightColor;
    types::Color         baseColor;
    std::array<float, 3> lightColorArray;
    std::array<float, 3> baseColorArray;
    float                command{0.0f};
};

}  // namespace micrasverse::physics

#endif  // MICRASVERSE_PHYSICS_ARGB_HPP
