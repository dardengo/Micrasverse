#ifndef SCENE_OBJ_HPP
#define SCENE_OBJ_HPP

#include "render/shader.hpp"
#include "models/rectangle.hpp"

#include "glm/glm.hpp"

namespace micrasverse::render {

class SceneObj {
public:
    Shader shader;
    Rectangle renderModel;
    
    SceneObj();
    virtual ~SceneObj();

    virtual void render(const glm::mat4 view, const glm::mat4 projection);
    
};

} // namespace micrasverse::render

#endif // SCENE_OBJ_HPP
