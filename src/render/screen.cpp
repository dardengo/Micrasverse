#include "render/screen.h"
#include "render/mouse.h"
#include "render/keyboard.h"

#include <iostream>

namespace micrasverse::render {

unsigned int Screen::SCR_WIDTH = 1366;
unsigned int Screen::SCR_HEIGHT = 768;

void Screen::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}

Screen::Screen() : window(nullptr) {}

bool Screen::init() {

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    //Set OpenGL version to 3.3 (fail if user doesn't have)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Set multisampling
    glfwWindowHint(GLFW_SAMPLES, 16);
    
    //Set OpenGL profile to core (not backwards compatible)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Micrasverse", NULL, NULL);

    if (!window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return false;
    }

    //Make the window the current context
    glfwMakeContextCurrent(window);

    // Enable V-Sync
    glfwSwapInterval(1);

    //Load GLAD and openGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD and openGl function pointers" << '\n';
        return false;
    }

    return true;
};

void Screen::setParameters() {
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetKeyCallback(window, Keyboard::key_callback);
    glfwSetCursorPosCallback(window, Mouse::cursor_position_callback);
    glfwSetMouseButtonCallback(window, Mouse::mouse_button_callback);
    glfwSetScrollCallback(window, Mouse::scroll_callback);
}

void Screen::update() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Screen::newFrame() {
    glfwSwapBuffers(window);
    glfwPollEvents();
}

bool Screen::shouldClose() {
    return glfwWindowShouldClose(window);
}

void Screen::setShouldClose(const bool shouldClose) {
    glfwSetWindowShouldClose(window, shouldClose);
}

GLFWwindow* Screen::getWindow() {
    return this->window;
}

} // micrasverse::render
