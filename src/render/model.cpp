#include "render/model.hpp"

namespace micrasverse::render {

Model::Model(Material material, glm::vec3 position, glm::vec3 size)
    : position(position), size(size), material(material) {}

void Model::init() {}

void Model::render(Shader shader, const bool setModel) {
    if (setModel) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::rotate(model, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, size);
        shader.setMat4("model", model);
    }

    for (Mesh mesh : meshes) {
        mesh.render();
    }
}

void Model::cleanUp() {
    for (Mesh mesh : meshes) {
        mesh.cleanUp();
    }
}

} // micrasverse::render
