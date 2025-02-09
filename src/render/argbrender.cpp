#include "render/argbrender.hpp"

namespace micrasverse::render {

ARGBRender::ARGBRender(const b2Vec2 position, const b2Vec2 size, const glm::vec3 lightColor, const glm::vec3 baseColor, const bool isOn)
    : SceneObj() {
    this->lightColor = lightColor;
    this->baseColor = baseColor;
    this->isOn = isOn;

    this->shader = Shader("./render/assets/vertex-core.glsl", "./render/assets/fragment-core.glsl");
    
    this->renderModel = Rectangle(
        glm::vec3(position.x, position.y, 0.02f),
        glm::vec3(size.x, size.y, 0.0f),
        this->isOn ? this->lightColor : this->baseColor
    );

    this->renderModel.init();
}

void ARGBRender::update(b2Vec2 position, b2Rot rotation, const glm::vec3 color, const bool isOn) {
    this->lightColor = color;
    this->renderModel.setColor(isOn ? this->lightColor : this->baseColor);
    this->renderModel.setPose(glm::vec3(position.x, position.y, 0.02f), rotation);
}

} // namespace micrasverse::render
