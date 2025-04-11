#ifndef LED_HPP
#define LED_HPP

#include "models/rectangle.hpp"

#include "glm/glm.hpp"

namespace micrasverse::render {

class Led : public Rectangle {
public:
    glm::vec3 lightColor;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    // Default constructor
    Led()
        : lightColor(glm::vec3(1.0f)),
        ambient(glm::vec3(0.1f)),
        diffuse(glm::vec3(0.5f)),
        specular(glm::vec3(0.5f)),
        Rectangle(Material::white_plastic, glm::vec3(0.0f), glm::vec3(1.0f)) {
        this->init();
        }

    Led(glm::vec3 lightColor,
        glm::vec3 ambient,
        glm::vec3 diffuse,
        glm::vec3 specular,
        glm::vec3 position,
        glm::vec3 size        )
        : lightColor(lightColor),
        ambient(ambient),
        diffuse(diffuse),
        specular(specular),
        Rectangle(Material::white_plastic, position, size) {
        this->init();
        }

    void render(Shader shader, const bool setModel = false) {
        // Set light color
        shader.set3Float("lightColor", lightColor);
           
        Rectangle::render(shader, setModel);
    }

    void setColor(glm::vec3 color) {
        this->lightColor = color;
        this->init();
    }

};
} // micrasverse::render

#endif // LED_HPP
