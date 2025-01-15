#ifndef GUI_HPP
#define GUI_HPP

#include "GUI/plot.hpp"
#include "physics/micrasbody.hpp"

#define GLFW_INCLUDE_NONE   // GLFW include guard
#include "glad/glad.h"
#include "GLFW/glfw3.h"


namespace micrasverse::render {

class GUI {
public:
    Plot plot;
    
    GUI();

    void init(GLFWwindow* window);
    
    void update();

    void draw(micrasverse::physics::MicrasBody& micrasBody);

    void render();
    
    void destroy();
};

} // namespace micrasverse::render

#endif // GUI_HPP
