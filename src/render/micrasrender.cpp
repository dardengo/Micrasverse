#include "render/micrasrender.hpp"
#include "render/material.hpp"

namespace micrasverse::render {

MicrasRender::MicrasRender(const b2Vec2 position, const b2Vec2 size)
    : SceneObj(), position(position), size(size) {
    this->shader = Shader();
    this->shader.generate("src/render/assets/vertex-core.glsl", "src/render/assets/fragment-core.glsl");
    
    this->renderModel = Rectangle(
        Material::green_pcb,
        glm::vec3(position.x , position.y, 0.02f),  // Position of the center
        glm::vec3(size.x, size.y, 0.01f)           // Size
    );

    this->renderModel.init();
}

void MicrasRender::update(const b2Vec2 position, const b2Rot rotation) {
    this->position = position;
    this->renderModel.setPose(glm::vec3(position.x, position.y, 0.02f), rotation);
}

BoundingBox MicrasRender::getBoundingBox() const {
    // Create a bounding box that encloses the micras robot
    // Add a small margin to ensure it's fully contained
    glm::vec3 min(
        position.x - size.x/2.0f - 0.05f,
        position.y - size.y/2.0f - 0.05f,
        0.0f
    );
    
    glm::vec3 max(
        position.x + size.x/2.0f + 0.05f,
        position.y + size.y/2.0f + 0.05f,
        0.05f
    );
    
    return BoundingBox(min, max);
}

} // namespace micrasverse::render
