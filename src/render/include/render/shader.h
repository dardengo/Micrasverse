#ifndef SHADER_H
#define SHADER_H

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

    Shader(const char* vertexShaderPath, const char* fragmentShaderPath);

    void generate (const char* vertexShaderPath, const char* fragmentShaderPath);

    void activate(const glm::mat4 view, const glm::mat4 projection);

    // Utility functions
    std::string loadShaderSrc(const char* filepath);
    GLuint compileShader(const char* filepath, GLenum type);
    
    // Set uniforms
    void setMat4(const std::string& name, const glm::mat4 value);

};

} // micrasverse::renderer

#endif // SHADER_H
