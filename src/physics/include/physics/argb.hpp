#ifndef ARGB_HPP
#define ARGB_HPP

#include "box2d/box2d.h"
#include "glm/glm.hpp"

namespace micrasverse::physics {

class Argb {
public:
    b2Vec2 worldPosition;
    b2Vec2 size;
    bool isOn {false};
    glm::vec3 baseColor;
    glm::vec3 lightColor;

    Argb(b2BodyId bodyId, b2Vec2 localPosition, b2Vec2 size, glm::vec3 color);

    void setColor(const glm::vec3 color);

    void update(b2Vec2 micrasPosition);

    void turnOn();

    void turnOn(const glm::vec3 color);

    void turnOff();

    void toggle();

private:
    b2BodyId bodyId;
    b2Vec2 localPosition;
    glm::vec3 mapColor(glm::vec3 color);
};


} // namespace micras::proxy

#endif // ARGB_HPP
