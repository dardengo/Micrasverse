#include "render/shader.hpp"

namespace micrasverse::render {

// Constructor
Shader::Shader() {}

void Shader::generate(const char* vertexShaderPath, const char* fragmentShaderPath) {
    
    // Compile vertex shader
    GLuint vertexShader = compileShader(vertexShaderPath, GL_VERTEX_SHADER);
    // Compile fragment shader
    GLuint fragmentShader = compileShader(fragmentShaderPath, GL_FRAGMENT_SHADER);

    // Create shader program
    id = glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    glLinkProgram(id);

    // Check if the shader program linked successfully
    int success;
    char infoLog[512];
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(id, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Delete the shaders as they are now linked to the program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use() {
    glUseProgram(this->id);
}

void Shader::activate(const glm::mat4 view, const glm::mat4 projection) {
    glUseProgram(this->id);
    this->setMat4("view", view);
    this->setMat4("projection", projection);
}

// Load shader source
std::string Shader::loadShaderSrc(const char* filepath){
    std::ifstream file {};
    std::stringstream buffer {};

    std::string shaderSource = "";

    file.open(filepath);

    if (file.is_open()) {
        buffer << file.rdbuf();
        shaderSource = buffer.str();
    } else {
        std::cerr << "Failed to open file: " << filepath << std::endl;
    }
    
    file.close();
    return shaderSource;
}

// Compile shader
GLuint Shader::compileShader(const char* filepath, GLenum type) {
    // Create shader
    GLuint shader = glCreateShader(type);

    // Load shader source
    std::string shaderSrc = loadShaderSrc(filepath);
    const GLchar* shaderSource = shaderSrc.c_str();
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);

    // Check if the shader compiled successfully
    int success {};
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return shader;
}

// Set uniforms
void Shader::setMat4(const std::string& name, const glm::mat4 value) {
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setFloat(const std::string& name, const float value) {
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::set3Float(const std::string& name, const glm::vec3 value) {
    set3Float(name, value.x, value.y, value.z);
}

void Shader::set3Float(const std::string& name, const float x, const float y, const float z) {
    glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
}

} // namespace micrasverse::renderer
