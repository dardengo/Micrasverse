#include "io/keyboard.hpp"

namespace micrasverse::io {

bool Keyboard::keys[GLFW_KEY_LAST] = {false};
bool Keyboard::keysChanged[GLFW_KEY_LAST] = {false};

void Keyboard::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action != GLFW_RELEASE) {
        if (!keys[key]) {
            keys[key] = true;
        }
    } else {
        keys[key] = false;
    }
    keysChanged[key] = action != GLFW_REPEAT;
}

bool Keyboard::key(int key) {
    return keys[key];
}

bool Keyboard::keyChanged(int key) {
    bool ret = keysChanged[key];
    keysChanged[key] = false;
    return ret;
}

bool Keyboard::keyWentUp(int key) {
    return !keys[key] && keyChanged(key);
}

bool Keyboard::keyWentDown(int key) {
    return keys[key] && keyChanged(key);
}

}  // namespace micrasverse::io
