#include "render/mesh.hpp"

namespace micrasverse::render {

std::vector<Vertex> Vertex::genList(float* vertices, int noVertices) {
    std::vector<Vertex> vertexList(noVertices);
    
    int stride = 6; // 3 position + 3 normal coordinates

    for (int i = 0; i < noVertices; i++) {
        vertexList[i].position = glm::vec3(vertices[i * stride + 0],
                                           vertices[i * stride + 1],
                                           vertices[i * stride + 2]
        );
        vertexList[i].normal = glm::vec3(vertices[i * stride + 3],
                                         vertices[i * stride + 4],
                                         vertices[i * stride + 5]
        );
        /*vertexList[i].color = glm::vec3(vertices[i * stride + 6],
                                        vertices[i * stride + 7],
                                        vertices[i * stride + 8]
        );*/
    }

    return vertexList;
}

Mesh::Mesh() {}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) : vertices(vertices), indices(indices) {
    setup();
}

void Mesh::render() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::cleanUp() {
    if (VAO != 0) {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        VAO = VBO = EBO = 0;
    }
}

void Mesh::setup() {
    // Generate OpenGL objects
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind vertex array object
    glBindVertexArray(VAO);

    // Vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // Element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // Normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    /*// Color attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(2);*/

    // Unbind VAO
    glBindVertexArray(0);
}

} // micrasverse::render
