#include "render/micrasrender.hpp"
#include "render/material.hpp"

namespace micrasverse::render {

MicrasRender::MicrasRender(const b2Vec2 position, const b2Vec2 size)
    : SceneObj() {
    this->shader = Shader();
    this->shader.generate("src/render/assets/vertex-core.glsl", "src/render/assets/fragment-core.glsl");
    
    this->renderModel = Rectangle(
        Material::green_plastic,
        glm::vec3(position.x , position.y, 0.02f),  // Position of the center
        glm::vec3(size.x, size.y, 0.01f)           // Size
    );

    this->renderModel.init();
}

void MicrasRender::update(const b2Vec2 position, const b2Rot rotation) {
    this->renderModel.setPose(glm::vec3(position.x, position.y, 0.02f), rotation);
}

} // namespace micrasverse::render
