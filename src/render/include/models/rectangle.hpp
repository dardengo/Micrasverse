#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "render/model.hpp"
#include "render/material.hpp"

#include "box2d/box2d.h"

namespace micrasverse::render {

class Rectangle : public Model {
public:
    Rectangle(Material material = Material::black_plastic, glm::vec3 position = glm::vec3(0.0f), glm::vec3 size = glm::vec3(1.0f))
        : Model(material, position, size) {}

    void init() {
        // Clear any previous meshes
        meshes.clear();
        
        // Simplify the calculation for vertices/indices
        float vertices[] = {
        // Positions        // Normals         
       -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  // Bottom left
        0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  // Bottom right
       -0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  // Top left
        0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // Top right
        };

        unsigned int indices[] = {
            0, 1, 2,  // First triangle
            1, 3, 2   // Second triangle
        };
        
        int noVertices = 4;
        int noIndices = 6;
        
        // Create vertices and indices lists from the arrays
        std::vector<Vertex> vertexList = Vertex::genList(vertices, noVertices);
        std::vector<unsigned int> indexList(indices, indices + noIndices);
        
        // Add the mesh to the model
        meshes.push_back(Mesh(vertexList, indexList));
    }

    void setPose(const glm::vec3 position, const b2Rot rotation) {
        this->position = position;
        this->rotation = b2Rot_GetAngle(rotation);
    }

    void setMaterial(const Material material) {
        this->material = material;
    }

    void setSize(const glm::vec3 size) {
        this->size = size;
    }
    
    void render(Shader shader, const bool setModel = false) {
        // Ensure shaders are properly set up before rendering
        if (meshes.empty()) {
            return; // Skip rendering if not initialized
        }
        
        // Set up the model matrix
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::rotate(model, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, size);

        // Set uniforms
        shader.setMat4("model", model);
        shader.set3Float("material.ambient", this->material.ambient);
        shader.set3Float("material.diffuse", this->material.diffuse);
        shader.set3Float("material.specular", this->material.specular);
        shader.setFloat("material.shininess", this->material.shininess);

        // Render the meshes
        for (Mesh& mesh : meshes) {
            mesh.render();
        }
    }
};

} // micrasverse::render

#endif // RECTANGLE_HPP
