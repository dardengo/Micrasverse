#ifndef MESH_H
#define MESH_H

#include "render/shader.h"

#define GLFW_INCLUDE_NONE   // GLFW include guard
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include <vector>

namespace micrasverse::render {

struct Vertex
{
    glm::vec3 position;
    glm::vec3 color;

    static std::vector<Vertex> genList(float* vertices, int noVertices);

};

class Mesh {
public:
    unsigned int VAO;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    Mesh();
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

    void render();

    void cleanUp();

private:
    unsigned int VBO;
    unsigned int EBO;

    void setup();
};

} // micrasverse::render
#endif // MESH_H
