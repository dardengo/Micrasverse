#ifndef ARGB_RENDER_HPP
#define ARGB_RENDER_HPP

#include "render/sceneobj.hpp"
#include "core/types.hpp"

#include <array>

namespace micrasverse::render {

class ARGBRender: public SceneObj {

public:
    bool isOn {false};
    glm::vec3 lightColor;
    glm::vec3 baseColor;

    ARGBRender(const b2Vec2 position, const b2Vec2 size, const glm::vec3 lightColor, const glm::vec3 baseColor, const bool isOn);
    ARGBRender(const b2Vec2 position, const b2Vec2 size, const std::array<float, 3>& lightColor, const std::array<float, 3>& baseColor, const bool isOn);

    //To-do: Change to template
    void update(b2Vec2 position, b2Rot rotation, const glm::vec3 color, const bool isOn);
    void update(b2Vec2 position, b2Rot rotation, const std::array<float, 3>& color, const bool isOn);

    glm::vec3 colorToVec3(const core::Color& color);
};


} // namespace micrasverse::render

#endif // ARGB_RENDER_HPP
