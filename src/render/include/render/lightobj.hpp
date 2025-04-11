#ifndef LIGHT_OBJ_HPP
#define LIGHT_OBJ_HPP

#include "render/shader.hpp"
#include "models/led.hpp"

#include "glm/glm.hpp"

namespace micrasverse::render {

class LightObj {
public:
    Shader shader;
    Led renderModel;
    
    LightObj();
    virtual ~LightObj();

    virtual void render(const glm::mat4 view, const glm::mat4 projection);
    
};

} // namespace micrasverse::render

#endif // LIGHT_OBJ_HPP
