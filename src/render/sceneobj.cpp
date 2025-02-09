#include "render/sceneObj.hpp"

namespace micrasverse::render {

SceneObj::SceneObj() {}

SceneObj::~SceneObj() {
    this->renderModel.cleanUp();
}

void SceneObj::render(const glm::mat4 view, const glm::mat4 projection) {
    this->shader.activate(view, projection);
    this->renderModel.render(this->shader, false);
}


} // namespace micrasverse::render
