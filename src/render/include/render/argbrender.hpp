#ifndef MICRAS_RENDER_ARGBRENDER_HPP
#define MICRAS_RENDER_ARGBRENDER_HPP

#include "render/lightobj.hpp"
#include "micrasverse_core/types.hpp"
#include "render/camera.hpp"

#include <array>

namespace micrasverse::render {

class ARGBRender: public LightObj {

public:
    bool isOn {false};
    glm::vec3 lightColor;
    glm::vec3 baseColor;
    Camera camera;

    ARGBRender(const b2Vec2 position, const b2Vec2 size, const glm::vec3 lightColor, const glm::vec3 baseColor, const bool isOn, const Camera& camera);
    ARGBRender(const b2Vec2 position, const b2Vec2 size, const std::array<float, 3>& lightColor, const std::array<float, 3>& baseColor, const bool isOn, const Camera& camera);

    //To-do: Change to template
    void update(b2Vec2 position, b2Rot rotation, const glm::vec3 color, const bool isOn);
    void update(b2Vec2 position, b2Rot rotation, const std::array<float, 3>& color, const bool isOn);

    glm::vec3 colorToVec3(const types::Color& color);

    void render(const glm::mat4 view, const glm::mat4 projection) override;
};


} // namespace micrasverse::render

#endif // MICRAS_RENDER_ARGBRENDER_HPP
