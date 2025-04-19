#ifndef MICRASVERSE_PHYSICS_ARGB_HPP
#define MICRASVERSE_PHYSICS_ARGB_HPP

#include "physics/i_actuator.hpp"
#include "physics/i_argb.hpp"
#include "micrasverse_core/types.hpp"
#include "box2d/box2d.h"
#include <memory>
#include <array>

namespace micrasverse::physics {

class Argb : public IArgb {
public:
    // Default constructor
    Argb() = default;
    
    // Constructor with parameters
    Argb(b2BodyId bodyId, b2Vec2 localPosition, b2Vec2 size, const types::Color& lightColor);
    
    virtual ~Argb() = default;

    // IArgb methods
    void update() override;
    void setColor(const types::Color& color) override;
    types::Color getColor() const override;
    void turnOn() override;
    void turnOn(const types::Color& color) override;
    void turnOff() override;
    void toggle() override;
    bool isOn() const override;
    types::Vec2 getWorldPosition() const override;
    types::Vec2 getSize() const override;
    void attachArgb(types::Vec2 localPosition, types::Vec2 size, types::Color color) override;
    void set_color(const types::Color& color, uint8_t index) override;
    void set_colors(const std::array<types::Color, 8>& colors) override;
    void encode_color(const types::Color& color, uint8_t index) override;
    
    // IActuator methods
    types::Vec2 getPosition() const override;
    void setCommand(float command) override;
    float getCommand() const override;
    bool isActive() const override;
    void update(float deltaTime) override;

    // Additional getter methods
    const std::array<float, 3>& getLightColorArray() const { return lightColorArray; }
    const std::array<float, 3>& getBaseColorArray() const { return baseColorArray; }

private:
    b2BodyId bodyId;
    b2Vec2 localPosition;
    b2Vec2 worldPosition;
    b2Vec2 size;
    bool isLedOn{false};
    types::Color lightColor;
    types::Color baseColor;
    std::array<float, 3> lightColorArray;
    std::array<float, 3> baseColorArray;
    float command{0.0f};
};

} // namespace micrasverse::physics

#endif // MICRASVERSE_PHYSICS_ARGB_HPP
