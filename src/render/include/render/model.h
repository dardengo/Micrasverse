#ifndef MODEL_H
#define MODEL_H

#include "render/mesh.h"

#define GLFW_INCLUDE_NONE   // GLFW include guard
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <vector>

namespace micrasverse::render {

class Model {
public:
    glm::vec3 position;
    glm::vec3 size;
    float rotation{0.0f};
    glm::vec3 color;

    Model(const glm::vec3 position = glm::vec3(0.0f), const glm::vec3 size = glm::vec3(1.0f), const glm::vec3 color = glm::vec3(1.0f));

    void init();

    void render(Shader shader, const bool setModel = true);

    void cleanUp();

protected:
    std::vector<Mesh> meshes;
};

} // micrasverse::render

#endif // MODEL_H
