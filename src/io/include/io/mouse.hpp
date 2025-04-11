#ifndef MOUSE_HPP
#define MOUSE_HPP

#define GLFW_INCLUDE_NONE   // GLFW include guard
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

namespace micrasverse::io {

class Mouse {
public:
    static void cursor_position_callback(GLFWwindow* window, double xPos, double yPos);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);

    static double getMouseX();
    static double getMouseY();

    static double getDX();
    static double getDY();

    static double getScrollDx();
    static double getScrollDy();

    static bool button(int button);
    static bool buttonChanged(int button);
    static bool buttonWentUp(int button);
    static bool buttonWentDown(int button);

private:

    static double x;
    static double y;

    static double lastX;
    static double lastY;

    static double dx;
    static double dy;

    static double scrollDx;
    static double scrollDy;

    static bool firstMouse;

    static bool buttons[];
    static bool buttonsChanged[];
};

} // micrasverse::io

#endif // MOUSE_HPP
