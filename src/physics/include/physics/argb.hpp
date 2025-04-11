#ifndef ARGB_HPP
#define ARGB_HPP

#include "core/types.hpp"

#include "box2d/box2d.h"

#include <array>

namespace micrasverse::physics {

class Argb {
public:
    b2Vec2 worldPosition;
    b2Vec2 size;
    bool isOn {false};
    core::Color baseColor;
    core::Color lightColor;
    std::array<float, 3> baseColorArray;
    std::array<float, 3> lightColorArray;

    Argb(b2BodyId bodyId, b2Vec2 localPosition, b2Vec2 size, core::Color& color);

    void setColor(const core::Color color);

    void update();

    void turnOn();

    void turnOn(const core::Color color);

    void turnOff();

    void toggle();

private:
    b2BodyId bodyId;
    b2Vec2 localPosition;
};


} // namespace micras::proxy

#endif // ARGB_HPP
