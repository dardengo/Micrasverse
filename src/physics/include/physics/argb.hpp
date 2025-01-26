#ifndef MICRAS_PROXY_ARGB_HPP
#define MICRAS_PROXY_ARGB_HPP

#include "models/rectangle.hpp"

#include "box2d/box2d.h"
#include "glm/glm.hpp"

#include <cstdint>
#include <vector>

namespace micrasverse::physics {

class Argb {
public:
    render::Rectangle argbRenderable;
    glm::vec3 lightColor;

    Argb(b2BodyId bodyId, b2Vec2 localPosition, b2Vec2 size, glm::vec3 color);

    void setColor(const glm::vec3);

    void update(b2Vec2 position, b2Rot rotation);

    void turnOn();

    void turnOn(const glm::vec3 color);

    void turnOff();

    void toggle();

private:
    b2BodyId bodyId;
    bool isOn {false};
    b2Vec2 localPosition;
    b2Vec2 worldPosition;
    b2Vec2 size;
    glm::vec3 baseColor;
    glm::vec3 mapColor(glm::vec3 color);
};


} // namespace micras::proxy

#endif // MICRAS_PROXY_ARGB_HPP
