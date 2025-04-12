#ifndef ARGB_HPP
#define ARGB_HPP

#include "physics/i_argb.hpp"
#include "box2d/box2d.h"
#include <array>

namespace micrasverse::physics {

class Argb : public IARGB {
public:
    // Box2D specific data kept public for now
    b2Vec2 worldPosition;
    b2Vec2 size;
    bool isLedOn {false};
    core::Color baseColor;
    core::Color lightColor;
    std::array<float, 3> baseColorArray;
    std::array<float, 3> lightColorArray;

    Argb(b2BodyId bodyId, b2Vec2 localPosition, b2Vec2 size, core::Color& color);

    // Implement IARGB interface
    void update() override;
    void setColor(const core::Color& color) override;
    core::Color getColor() const override;
    void turnOn() override;
    void turnOn(const core::Color& color) override;
    void turnOff() override;
    void toggle() override;
    bool isOn() const override;
    core::Vec2 getWorldPosition() const override;
    core::Vec2 getSize() const override;

private:
    b2BodyId bodyId;
    b2Vec2 localPosition;
};

} // namespace micrasverse::physics

#endif // ARGB_HPP
