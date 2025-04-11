#include "render/lightobj.hpp"

namespace micrasverse::render {

LightObj::LightObj() {}

LightObj::~LightObj() {
    this->renderModel.cleanUp();
}

void LightObj::render(const glm::mat4 view, const glm::mat4 projection) {
    this->shader.activate(view, projection);
    this->renderModel.render(this->shader, false);
}


} // namespace micrasverse::render
