#include "render/argbrender.hpp"
#include "models/led.hpp"

namespace micrasverse::render {

ARGBRender::ARGBRender(const b2Vec2 position, const b2Vec2 size, const glm::vec3 lightColor, const glm::vec3 baseColor, const bool isOn, const Camera& camera)
    : LightObj(), camera(camera) {
    this->lightColor = lightColor;
    this->baseColor = baseColor;
    this->isOn = isOn;

    this->shader = Shader();
    this->shader.generate("src/render/assets/vertex-core.glsl", "src/render/assets/fragment-led.glsl");
    
    this->renderModel = Led(
        isOn ? this->lightColor : this->baseColor,
        glm::vec3(1.0f),
        glm::vec3(1.0f),
        glm::vec3(1.0f),
        glm::vec3(position.x, position.y, 0.021f),
        glm::vec3(size.x, size.y, 0.01f)
    );

    this->renderModel.init();
}

//To-do: Change to template
ARGBRender::ARGBRender(const b2Vec2 position, const b2Vec2 size, const std::array<float, 3>& lightColor, const std::array<float,3>& baseColor, const bool isOn, const Camera& camera)
    : LightObj(), camera(camera) {
    this->lightColor = {lightColor[0], lightColor[1], lightColor[2]};
    this->baseColor = {baseColor[0], baseColor[1], baseColor[2]};
    this->isOn = isOn;

    this->shader = Shader();
    this->shader.generate("src/render/assets/vertex-core.glsl", "src/render/assets/fragment-led.glsl");
    
    this->renderModel = Led(
        isOn ? this->lightColor : this->baseColor,
        glm::vec3(1.0f),
        glm::vec3(1.0f),
        glm::vec3(1.0f),
        glm::vec3(position.x, position.y, 0.02f),
        glm::vec3(size.x, size.y, 0.01f)
    );

    this->renderModel.init();
}

void ARGBRender::update(b2Vec2 position, b2Rot rotation, const glm::vec3 color, const bool isOn) {
    this->lightColor = color;
    this->renderModel.setColor(isOn ? this->lightColor : this->baseColor);
    this->renderModel.setPose(glm::vec3(position.x, position.y, 0.02f), rotation);
}

void ARGBRender::update(b2Vec2 position, b2Rot rotation, const std::array<float, 3>& color, const bool isOn) {
    this->lightColor = {color[0], color[1], color[2]};
    this->renderModel.setColor(isOn ? this->lightColor : this->baseColor);
    this->renderModel.setPose(glm::vec3(position.x, position.y, 0.02f), rotation);
}

glm::vec3 ARGBRender::colorToVec3(const types::Color& color) {
    return glm::vec3(color.r, color.g, color.b);
}

void ARGBRender::render(const glm::mat4 view, const glm::mat4 projection) {
    // Add depth test disable to ensure LEDs are always visible on top
    glDisable(GL_DEPTH_TEST);
    
    this->shader.set3Float("lightPosition", this->renderModel.position);
    this->shader.set3Float("viewPosition", this->camera.position);
    this->shader.set3Float("light.ambient", this->renderModel.ambient);
    this->shader.set3Float("light.diffuse", this->renderModel.diffuse);
    this->shader.set3Float("light.specular", this->renderModel.specular);
    
    this->shader.activate(view, projection);
    
    this->renderModel.render(shader, true);
    
    // Restore depth testing for other objects
    glEnable(GL_DEPTH_TEST);
}

} // namespace micrasverse::render
