#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "render/model.hpp"
#include "box2d/box2d.h"

namespace micrasverse::render {

class Rectangle : public Model {
public:

    Rectangle(const glm::vec3 position = glm::vec3(0.0f), const glm::vec3 size = glm::vec3(1.0f), const glm::vec3 color = glm::vec3(1.0f))
        : Model(position, size, color) {}

    void init() {
        int noVertices {0};

        float vertices[] = {
        //Positions         // Colors
        -0.5f, -0.5f, 0.0f, color[0], color[1], color[2], //bottom left
         0.5f, -0.5f, 0.0f, color[0], color[1], color[2], //bottom right
        -0.5f,  0.5f, 0.0f, color[0], color[1], color[2], //top left
         0.5f, -0.5f, 0.0f, color[0], color[1], color[2], //bottom right
        -0.5f,  0.5f, 0.0f, color[0], color[1], color[2], //top left
         0.5f,  0.5f, 0.0f, color[0], color[1], color[2]  //top right
        };

        noVertices = sizeof(vertices) / sizeof(float) / 3;

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

    void setColor(const glm::vec3 color) {
        this->color = color;
        this->init();
    }

    void setSize(const glm::vec3 size) {
        this->size = size;
    }
    
    void render(Shader shader, const bool setModel = false) {
        glm::mat4 model = glm::mat4(1.0f);

        //Set position
        model = glm::translate(model, glm::vec3(position.x , position.y, position.z));

        // Rotate
        model = glm::rotate(model, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
        
        // Scale
        model = glm::scale(model, size);

        shader.setMat4("model", model);

        Model::render(shader, setModel);
    }
    
};

} // micrasverse::render

#endif // RECTANGLE_HPP
