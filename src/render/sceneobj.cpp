#include "render/sceneobj.hpp"

namespace micrasverse::render {

SceneObj::SceneObj() {}

SceneObj::~SceneObj() {
    this->renderModel.cleanUp();
}

void SceneObj::render(const glm::mat4 view, const glm::mat4 projection, glm::vec3 position, glm::vec3 cameraPosition, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) {
    this->shader.activate(view, projection);

    shader.set3Float("lightPosition", position);
    shader.set3Float("viewPosition", cameraPosition);
    shader.set3Float("light.ambient", ambient);
    shader.set3Float("light.diffuse", diffuse);
    shader.set3Float("light.specular", specular);

    this->renderModel.render(this->shader, false);
}


} // namespace micrasverse::render
