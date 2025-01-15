#ifndef SCREEN_HPP
#define SCREEN_HPP

#include "render/camera.hpp"
#include "GUI/gui.hpp"
#include "physics/rectanglebody.hpp"

#define GLFW_INCLUDE_NONE   // GLFW include guard
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace micrasverse::render {

class Screen {
private:
    GLFWwindow* window;
    GLFWmonitor* monitor;
    const GLFWvidmode* mode;
    Camera camera;
    GUI gui;

public:
    static unsigned int SCR_WIDTH;
    static unsigned int SCR_HEIGHT;
    glm::mat4 view;
    glm::mat4 projection;    
      
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    
    Screen();

    ~Screen();

    bool init();

    void setParameters();

    void processInput(const float deltaTime);

    void update(const micrasverse::physics::MicrasBody& micrasBody);

    void renderGUI(micrasverse::physics::MicrasBody& micrasBody);

    void newFrame();

    void destroy();

    bool shouldClose();

    void setShouldClose(const bool shouldClose);

    GLFWwindow* getWindow();


};


} // namespace micrasverse::render

#endif // SCREEN_HPP
