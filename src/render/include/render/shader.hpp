#ifndef SHADER_HPP
#define SHADER_HPP

#define GLFW_INCLUDE_NONE   // GLFW include guard
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


namespace micrasverse::render {

class Shader {
public:
    unsigned int id;

    Shader();

    void generate(const char* vertexShaderPath, const char* fragmentShaderPath);

    // Activate the shader
    void use();
    
    // Activate with view and projection matrices
    void activate(const glm::mat4 view, const glm::mat4 projection);

    // Utility functions
    std::string loadShaderSrc(const char* filepath);
    GLuint compileShader(const char* filepath, GLenum type);
    
    // Set uniforms
    void setMat4(const std::string& name, const glm::mat4 value);
    void setFloat(const std::string& name, const float value);
    void set3Float(const std::string& name, const glm::vec3 value);
    void set3Float(const std::string& name, const float x, const float y, const float z);
};

} // micrasverse::renderer

#endif // SHADER_HPP
