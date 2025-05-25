#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include <GLFW/glfw3.h>

namespace micrasverse::io {

class Keyboard {
public:
    static bool key(int key);
    static bool keyChanged(int key);
    static bool keyWentUp(int key);
    static bool keyWentDown(int key);

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

private:
    static bool keys[];
    static bool keysChanged[];
};

}  // namespace micrasverse::io

#endif  // KEYBOARD_HPP
