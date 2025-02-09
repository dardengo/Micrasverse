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
        int noVertices {0};

        float vertices[] = {
        // Positions        // Normals         
       -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  // Bottom left
        0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  // Bottom right
       -0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  // Top left
        0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  // Bottom right
       -0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  // Top left
        0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  // Top right
        };

        noVertices = sizeof(vertices) / sizeof(float) / 6;

        std::vector<unsigned int> indices(noVertices);

        for (unsigned int i = 0; i < noVertices; i++) {
            indices[i] = i;
        }

        meshes.push_back(Mesh(Vertex::genList(vertices, noVertices), indices));
    }

    void setPose(const glm::vec3 position, const b2Rot rotation) {
        this->position = position;
        this->rotation = b2Rot_GetAngle(rotation);
    }

    void setMaterial(const Material material) {
        this->material = material;
        this->init();
    }

    void setSize(const glm::vec3 size) {
        this->size = size;
    }
    
    void render(Shader shader, const bool setModel = false) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(position.x , position.y, position.z));
        model = glm::rotate(model, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, size);

        shader.setMat4("model", model);

        shader.set3Float("material.ambient", this->material.ambient);
        shader.set3Float("material.diffuse", this->material.diffuse);
        shader.set3Float("material.specular", this->material.specular);
        shader.setFloat("material.shininess", this->material.shininess);

        Model::render(shader, setModel);
    }
    
};

} // micrasverse::render

#endif // RECTANGLE_HPP
