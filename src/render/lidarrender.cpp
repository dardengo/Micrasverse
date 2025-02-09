#include "render/lidarrender.hpp"
#include "render/material.hpp"
#include "models/led.hpp"

namespace micrasverse::render {

LidarRender::LidarRender(const b2Vec2 rayMidPoint, const b2Rot rayDirection, const float reading, const Camera& camera)
    : LightObj(), lightColor(glm::vec3(237.0f, 47.0f, 50.0f) / 255.0f), camera(camera) {

    this->shader = Shader("./render/assets/vertex-core.glsl", "./render/assets/fragment-led.glsl");
    this->renderModel = Led(
        this->lightColor,
        glm::vec3(1.0f),
        glm::vec3(1.0f),
        glm::vec3(1.0f),
        glm::vec3(rayMidPoint.x, rayMidPoint.y, 0.02f),
        glm::vec3(0.002f, reading, 0.02f)
    );
    
    this->renderModel.init();
}

void LidarRender::update(const b2Vec2 rayMidPoint, const b2Rot rayDirection, const float reading) {
    this->renderModel.setColor(glm::vec3(237.0f, 47.0f, 50.0f) / 255.0f);
    this->renderModel.setPose(glm::vec3(rayMidPoint.x, rayMidPoint.y, 0.02f), rayDirection);
    this->renderModel.setSize(glm::vec3(0.002f, reading, 0.02f));
}

void LidarRender::render(const glm::mat4 view, const glm::mat4 projection) {
    this->shader.set3Float("lightPosition", this->renderModel.position);
    this->shader.set3Float("viewPosition", this->camera.position);
    
    this->shader.set3Float("light.ambient", this->renderModel.ambient);
    this->shader.set3Float("light.diffuse", this->renderModel.diffuse);
    this->shader.set3Float("light.specular", this->renderModel.specular);
    
    this->shader.activate(view, projection);
    
    this->renderModel.render(shader, true);
}

} // namespace micrasverse::render
