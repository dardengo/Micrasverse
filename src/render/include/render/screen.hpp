#ifndef SCREEN_HPP
#define SCREEN_HPP

#include "simulation/simulation_engine.hpp"

#include "render/camera.hpp"
#include "physics/box2d_rectanglebody.hpp"

#define GLFW_INCLUDE_NONE   // GLFW include guard
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <memory>

namespace micrasverse::render {
    class GUI;  // Forward declaration
    class RenderEngine;
}

namespace micrasverse::render {

class Screen {
private:
    GLFWwindow* window;
    GLFWmonitor* monitor;
    const GLFWvidmode* mode;
    std::unique_ptr<GUI> gui;
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

    void setProxyBridge(const std::shared_ptr<micras::ProxyBridge>& proxyBridge);
    void setRenderEngine(RenderEngine* renderEngine);
    
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    
    bool init();

    void setParameters();

    void processInput();
    
    void toggleFullscreen();

    void update(const micrasverse::physics::Box2DMicrasBody& micrasBody);

    void renderGUI(micrasverse::physics::Box2DMicrasBody& micrasBody);

    void newFrame();

    void destroy();

    bool shouldClose();

    void setShouldClose(const bool shouldClose);

    GLFWwindow* getWindow();

    std::unique_ptr<GUI>& getGUI() { return gui; }

};


} // namespace micrasverse::render

#endif // SCREEN_HPP
