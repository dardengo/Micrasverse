#ifndef MESH_HPP
#define MESH_HPP

#include "render/shader.hpp"

#define GLFW_INCLUDE_NONE   // GLFW include guard
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include <vector>

namespace micrasverse::render {

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;

    static std::vector<Vertex> genList(float* vertices, int noVertices);

};

class Mesh {
public:
    unsigned int VAO = 0;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    Mesh();
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

    void render();

    void cleanUp();

private:
    unsigned int VBO = 0;
    unsigned int EBO = 0;

    void setup();
};

} // micrasverse::render
#endif // MESH_HPP
