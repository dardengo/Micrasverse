#include "render/micrasrender.hpp"

namespace micrasverse::render {

MicrasRender::MicrasRender(const b2Vec2 position, const b2Vec2 size)
    : SceneObj() {
    this->shader = Shader("./render/assets/vertex-core.glsl", "./render/assets/fragment-core.glsl");
    
    this->renderModel = Rectangle(
        glm::vec3(position.x , position.y, 0.02f),  // Position of the center
        glm::vec3(size.x, size.y, 0.01f),           // Size
        glm::vec3(15.0f, 125.0f, 15.0f) / 255.0f    // Color (RGB from 0 to 1)
    );

    this->renderModel.init();
}

void MicrasRender::update(const b2Vec2 position, const b2Rot rotation) {
    this->renderModel.setPose(glm::vec3(position.x, position.y, 0.02f), rotation);
}

} // namespace micrasverse::render
