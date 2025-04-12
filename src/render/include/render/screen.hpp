#ifndef SCREEN_HPP
#define SCREEN_HPP

#include "simulation/simulation_engine.hpp"

#include "render/camera.hpp"
#include "GUI/gui.hpp"
#include "physics/box2d_rectanglebody.hpp"

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
    GUI gui;
    std::shared_ptr<micrasverse::simulation::SimulationEngine> simulationEngine;
    
    // FPS counting
    float fps = 0.0f;
    float lastFrameTime;
    int frameCount;
    
    // Fullscreen tracking
    bool isFullscreen;
    int lastWidth;
    int lastHeight;
    
    void updateAndDisplayFPS();
    
public:
    Camera camera;
    static unsigned int SCR_WIDTH;
    static unsigned int SCR_HEIGHT;
    glm::mat4 view;
    glm::mat4 projection;    
    float deltaTime{ 0.0f };
    float lastFrame{ 0.0f };
      
    
    Screen(const std::shared_ptr<micrasverse::simulation::SimulationEngine>& simulationEngine);
    
    ~Screen();
    
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    
    bool init();

    void setParameters();

    void processInput();
    
    void toggleFullscreen();

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
