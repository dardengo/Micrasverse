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

    virtual void render(const glm::mat4 view, const glm::mat4 projection, glm::vec3 position, glm::vec3 cameraPosition, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
    
};

} // namespace micrasverse::render

#endif // SCENE_OBJ_HPP
