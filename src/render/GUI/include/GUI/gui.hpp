#ifndef GUI_HPP
#define GUI_HPP

#include "simulation/simulation_engine.hpp"
#include "GUI/plot.hpp"
#include "physics/box2d_micrasbody.hpp"
#include "micras/proxy/proxy_bridge.hpp"

#define GLFW_INCLUDE_NONE   // GLFW include guard
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <vector>
#include <string>
#include <memory>

namespace micrasverse::render {

class GUI {
public:
    GUI();
    ~GUI() = default;

    void setSimulationEngine(const std::shared_ptr<micrasverse::simulation::SimulationEngine>& simulationEngine);
    void setProxyBridge(const std::shared_ptr<micras::ProxyBridge>& proxyBridge);
    void init(GLFWwindow* window);
    void update();
    void draw(micrasverse::physics::Box2DMicrasBody& micrasBody);
    void render();
    void destroy();

    bool isProxyBridgeInitialized() const { return proxyBridge != nullptr; }

private:
    bool showStyleEditor;
    GLFWwindow* currentWindow;
    std::shared_ptr<micrasverse::simulation::SimulationEngine> simulationEngine;
    std::shared_ptr<micras::ProxyBridge> proxyBridge;
    Plot plot;
};

} // namespace micrasverse::render

#endif // GUI_HPP
