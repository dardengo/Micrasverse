#include "render/lidarrender.hpp"

namespace micrasverse::render {

LidarRender::LidarRender(const b2Vec2 rayMidPoint, const b2Rot rayDirection, const float reading)
    : SceneObj() {

    this->shader = Shader("./render/assets/vertex-core.glsl", "./render/assets/fragment-core.glsl");
    this->renderModel = Rectangle(
        glm::vec3(0.0f),
        glm::vec3(0.0f),
        glm::vec3(237.0f, 47.0f, 50.0f) / 255.0f
    );
    
    this->renderModel.init();
}

void LidarRender::update(const b2Vec2 rayMidPoint, const b2Rot rayDirection, const float reading) {
    this->renderModel.setColor(glm::vec3(237.0f, 47.0f, 50.0f) / 255.0f);
    this->renderModel.setPose(glm::vec3(rayMidPoint.x, rayMidPoint.y, 0.02f), rayDirection);
    this->renderModel.setSize(glm::vec3(0.002f, reading, 0.02f));
}

} // namespace micrasverse::render
