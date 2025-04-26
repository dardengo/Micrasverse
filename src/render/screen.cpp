#include "render/screen.hpp"
#include "io/mouse.hpp"
#include "io/keyboard.hpp"
#include "GUI/gui.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <algorithm>

namespace micrasverse::render {

unsigned int Screen::SCR_WIDTH = 1366;
unsigned int Screen::SCR_HEIGHT = 768;


Screen::Screen(const std::shared_ptr<micrasverse::simulation::SimulationEngine>& simulationEngine) 
    : window(nullptr), camera(), simulationEngine(simulationEngine), 
      lastFrameTime(0.0f), frameCount(0), isFullscreen(false), lastWidth(SCR_WIDTH), lastHeight(SCR_HEIGHT) {
    this->gui = std::make_unique<GUI>();
}

Screen::~Screen() {
    this->destroy();
}

void Screen::setProxyBridge(const std::shared_ptr<micras::ProxyBridge>& proxyBridge) {
    gui->setProxyBridge(proxyBridge);
}

void Screen::setRenderEngine(RenderEngine* renderEngine) {
    gui->setRenderEngine(renderEngine);
}

void Screen::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}

bool Screen::init() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    //Set OpenGL version to 3.3 (fail if user doesn't have)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Set multisampling (reduced from 16 to 4 samples)
    // Use dynamic multisampling based on screen size
    int samples = 4;
    glfwWindowHint(GLFW_SAMPLES, samples);
    
    //Set OpenGL profile to core (not backwards compatible)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

    // Enable depth testing for proper 3D rendering
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    // Enable multisampling for smoother edges
    glEnable(GL_MULTISAMPLE);

    // Initialize view and projection matrices
    this->view = glm::mat4(1.0f);
    this->projection = glm::mat4(1.0f);

    return true;
};

void Screen::setParameters() {
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT); //for windowed
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetKeyCallback(window, io::Keyboard::key_callback);
    glfwSetCursorPosCallback(window, io::Mouse::cursor_position_callback);
    glfwSetMouseButtonCallback(window, io::Mouse::mouse_button_callback);
    glfwSetScrollCallback(window, io::Mouse::scroll_callback);

    // Create and initialize GUI after user callbacks are set so to not overwrite ImGui's callbacks 
    this->gui->setSimulationEngine(this->simulationEngine);
    this->gui->init(this->window);
}

void Screen::processInput() {
    // compute delta time
    float currentFrame = glfwGetTime();
    this->deltaTime = currentFrame - this->lastFrame;
    this->lastFrame = currentFrame;

    if (io::Keyboard::keyWentDown(GLFW_KEY_ESCAPE)) {
        this->setShouldClose(true);
    }
    
    if (io::Keyboard::keyWentDown(GLFW_KEY_F)) {
        this->camera.followMicras = !this->camera.followMicras;
    }

    // Center the camera on the maze when 'C' is pressed
    if (io::Keyboard::keyWentDown(GLFW_KEY_C)) {
        this->camera.followMicras = false;
        this->camera.position = glm::vec3(-0.3f, micrasverse::MAZE_FLOOR_HALFHEIGHT, 4.75f);
    }
    
    // Toggle fullscreen when F11 is pressed
    if (io::Keyboard::keyWentDown(GLFW_KEY_F11)) {
        toggleFullscreen();
    }

    // Only process camera movement if ImGui is not capturing keyboard input
    if (!ImGui::GetIO().WantCaptureKeyboard) {
        if (io::Keyboard::key(GLFW_KEY_UP)) {
            this->camera.updateCameraPos(CameraDirection::UP, deltaTime);
        }

        if (io::Keyboard::key(GLFW_KEY_DOWN)) {
            this->camera.updateCameraPos(CameraDirection::DOWN, deltaTime);
        }

        if (io::Keyboard::key(GLFW_KEY_LEFT)) {
            this->camera.updateCameraPos(CameraDirection::LEFT, deltaTime);
        }

        if (io::Keyboard::key(GLFW_KEY_RIGHT)) {
            this->camera.updateCameraPos(CameraDirection::RIGHT, deltaTime);
        }
    }

    double scrollDy = io::Mouse::getScrollDy();
    if ((scrollDy != 0.0) & (!ImGui::GetIO().WantCaptureMouse)) {
        this->camera.updateCameraZoom(scrollDy);
    }
}

void Screen::toggleFullscreen() {
    isFullscreen = !isFullscreen;
    
    if (isFullscreen) {
        // Store current window dimensions
        glfwGetWindowSize(window, &lastWidth, &lastHeight);
        
        // Get primary monitor
        GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
        
        // Make window fullscreen
        glfwSetWindowMonitor(window, primaryMonitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        
        // Adjust multisampling and rendering optimizations for fullscreen
        glDisable(GL_MULTISAMPLE);
    } else {
        // Restore windowed mode
        glfwSetWindowMonitor(window, nullptr, 100, 100, lastWidth, lastHeight, GLFW_DONT_CARE);
        
        // Restore higher quality rendering for windowed mode
        glEnable(GL_MULTISAMPLE);
    }
    
    // Update viewport
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
}

void Screen::updateAndDisplayFPS() {
    // Update FPS counter
    float currentTime = glfwGetTime();
    frameCount++;
    
    // Update FPS every second
    if (currentTime - lastFrameTime >= 1.0f) {
        fps = static_cast<float>(frameCount) / (currentTime - lastFrameTime);
        frameCount = 0;
        lastFrameTime = currentTime;
        
        // Display FPS in window title
        char title[256];
        snprintf(title, sizeof(title), "Micrasverse | FPS: %.1f", fps);
        glfwSetWindowTitle(window, title);
    }
}

void Screen::update(const micrasverse::physics::Box2DMicrasBody& micrasBody) {
    // Clear the screen
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Apply performance optimizations for fullscreen mode
    if (isFullscreen && SCR_WIDTH > 1600) {
        // Skip some CPU-intensive operations in fullscreen at higher resolutions
        glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);
        glHint(GL_FRAGMENT_SHADER_DERIVATIVE_HINT, GL_FASTEST);
    } else {
        glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
        glHint(GL_FRAGMENT_SHADER_DERIVATIVE_HINT, GL_NICEST);
    }

    // Update GUI
    this->gui->update();

    // Update camera
    this->camera.update(micrasBody);

    // Update view matrix
    this->view = this->camera.getViewMatrix();
    
    // Update projection matrix
    this->projection = glm::perspective(glm::radians(this->camera.getZoom()),
                                        (float)SCR_WIDTH / (float)SCR_HEIGHT,
                                        0.1f,
                                        100.0f);
    
    // Update FPS counter
    updateAndDisplayFPS();
}

void Screen::renderGUI(micrasverse::physics::Box2DMicrasBody& micrasBody) {
    // Now render GUI
    this->gui->draw(micrasBody);
    this->gui->render();
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
    this->gui->destroy();
    glfwTerminate();
}

} // micrasverse::render
