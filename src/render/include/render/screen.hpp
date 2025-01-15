#ifndef SCREEN_HPP
#define SCREEN_HPP


#define GLFW_INCLUDE_NONE   // GLFW include guard
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace micrasverse::render {

class Screen {
public:
    static unsigned int SCR_WIDTH;
    static unsigned int SCR_HEIGHT;
      
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    
    Screen();

    bool init();

    void setParameters();


    void newFrame();

    bool shouldClose();

    void setShouldClose(const bool shouldClose);

    GLFWwindow* getWindow();


};


} // namespace micrasverse::render

#endif // SCREEN_HPP
