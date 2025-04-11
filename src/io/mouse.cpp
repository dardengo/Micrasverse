#include "io/mouse.hpp"

namespace micrasverse::io {

double Mouse::x = 0;
double Mouse::y = 0;

double Mouse::lastX = 0;
double Mouse::lastY = 0;

double Mouse::dx = 0;
double Mouse::dy = 0;

double Mouse::scrollDx = 0;
double Mouse::scrollDy = 0;

bool Mouse::firstMouse = true;

bool Mouse::buttons[GLFW_MOUSE_BUTTON_LAST] = {false};
bool Mouse::buttonsChanged[GLFW_MOUSE_BUTTON_LAST] = {false};

void Mouse::cursor_position_callback(GLFWwindow* window, double xPos, double yPos){

    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    x = xPos;
    y = yPos;
    
    dx = xPos - lastX;
    dy = lastY - yPos;

    lastX = xPos;
    lastY = yPos;
}

void Mouse::mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    if (action != GLFW_RELEASE) {
        if (!buttons[button]) {
            buttons[button] = true;
        }
    } else {
        buttons[button] = false;
    }

    buttonsChanged[button] = action != GLFW_REPEAT;
}

void Mouse::scroll_callback(GLFWwindow* window, double xOffset, double yOffset){
    scrollDx = xOffset;
    scrollDy = yOffset;
}

double Mouse::getMouseX(){
    return x;
}

double Mouse::getMouseY(){
    return y;
}

double Mouse::getDX(){
    double _dx = dx;
    dx = 0;
    return _dx;
}

double Mouse::getDY(){
    double _dy = dy;
    dy = 0;
    return _dy;
}

double Mouse::getScrollDx(){
    double _scrollDx = scrollDx;
    scrollDx = 0;
    return _scrollDx;
}

double Mouse::getScrollDy(){
    double _scrollDy = scrollDy;
    scrollDy = 0;
    return _scrollDy;
}

bool Mouse::button(int button){
    return buttons[button];
}

bool Mouse::buttonChanged(int button){
    bool ret = buttonsChanged[button];
    buttonsChanged[button] = false;
    return ret;
}

bool Mouse::buttonWentUp(int button){
    return !buttons[button] && buttonChanged(button);
}

bool Mouse::buttonWentDown(int button){
    return buttons[button] && buttonChanged(button);
}



} // micrasverse::io
