#ifndef ARGB_RENDER_HPP
#define ARGB_RENDER_HPP

#include "render/sceneobj.hpp"

namespace micrasverse::render {

class ARGBRender: public SceneObj {

public:
    bool isOn {false};
    glm::vec3 lightColor;
    glm::vec3 baseColor;

    ARGBRender(const b2Vec2 position, const b2Vec2 size, const glm::vec3 lightColor, const glm::vec3 baseColor, const bool isOn);

    void update(b2Vec2 position, b2Rot rotation, const glm::vec3 color, const bool isOn);
};


} // namespace micrasverse::render

#endif // ARGB_RENDER_HPP
