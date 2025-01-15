#include "render/screen.hpp"
#include "render/mouse.hpp"
#include "render/keyboard.hpp"

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

Screen::~Screen() {
    this->destroy();
}

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

    /*// Set up windowed fullscreen mode
    this->monitor = glfwGetPrimaryMonitor();
    this->mode = glfwGetVideoMode(monitor);
 
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    
    this->window = glfwCreateWindow(mode->width, mode->height, "My Title", monitor, NULL);*/

    this->window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Micrasverse", NULL, NULL);

    if (!this->window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return false;
    }

    //Make the window the current context
    glfwMakeContextCurrent(this->window);

    // Enable V-Sync
    glfwSwapInterval(1);

    //Load GLAD and openGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD and openGl function pointers" << '\n';
        return false;
    }

    // Initialize view and projection matrices
    this->view = glm::mat4(1.0f);
    this->projection = glm::mat4(1.0f);

    // Create and initialize camera
    micrasverse::render::Camera camera(glm::vec3(-0.3f, micrasverse::MAZE_FLOOR_HALFHEIGHT, 4.75f));
    this->camera = camera;

    return true;
};

void Screen::setParameters() {
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT); //for windowed
    //glViewport(0, 0, mode->width, mode->height); //for windowed fullscreen
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetKeyCallback(window, Keyboard::key_callback);
    glfwSetCursorPosCallback(window, Mouse::cursor_position_callback);
    glfwSetMouseButtonCallback(window, Mouse::mouse_button_callback);
    glfwSetScrollCallback(window, Mouse::scroll_callback);


    // Create and initialize GUI after user callbacks are set so to not overwrite ImGui's callbacks 
    GUI gui;
    this->gui = gui;
    this->gui.init(this->window);
}

void Screen::processInput(const float deltaTime) {
    if (micrasverse::render::Keyboard::keyWentDown(GLFW_KEY_ESCAPE)) {
        this->setShouldClose(true);
    }
    
    if (micrasverse::render::Keyboard::keyWentDown(GLFW_KEY_F)) {
        this->camera.followMicras = !this->camera.followMicras;
    }

    // Center the camera on the maze when 'C' is pressed
    if (micrasverse::render::Keyboard::keyWentDown(GLFW_KEY_C)) {
        this->camera.followMicras = false;
        this->camera.position = glm::vec3(-0.3f, micrasverse::MAZE_FLOOR_HALFHEIGHT, 4.75f);
    }

    if (Keyboard::key(GLFW_KEY_UP)) {
        this->camera.updateCameraPos(CameraDirection::UP, deltaTime);
    }

    if (Keyboard::key(GLFW_KEY_DOWN)) {
        this->camera.updateCameraPos(CameraDirection::DOWN, deltaTime);
    }

    if (Keyboard::key(GLFW_KEY_LEFT)) {
        this->camera.updateCameraPos(CameraDirection::LEFT, deltaTime);
    }

    if (Keyboard::key(GLFW_KEY_RIGHT)) {
        this->camera.updateCameraPos(CameraDirection::RIGHT, deltaTime);
    }

    double scrollDy = Mouse::getScrollDy();
    if (scrollDy != 0.0) {
        this->camera.updateCameraZoom(scrollDy);
    }
}

void Screen::update(const micrasverse::physics::MicrasBody& micrasBody) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Update GUI
    this->gui.update();

    // Update camera
    this->camera.update(micrasBody);

    // Update view matrix
    this->view = this->camera.getViewMatrix();
    
    // Update projection matrix
    this->projection = glm::perspective(glm::radians(this->camera.getZoom()), (float)micrasverse::render::Screen::SCR_WIDTH / (float)micrasverse::render::Screen::SCR_HEIGHT, 0.1f, 100.0f);

}

void Screen::renderGUI(micrasverse::physics::MicrasBody& micrasBody) {
    this->gui.draw(micrasBody);
    this->gui.render();
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

void Screen::destroy() {
    this->gui.destroy();
    glfwTerminate();
}

} // micrasverse::render
