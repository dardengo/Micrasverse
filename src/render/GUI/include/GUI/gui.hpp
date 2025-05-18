#ifndef GUI_HPP
#define GUI_HPP

#include "simulation/simulation_engine.hpp"
#include "GUI/plot.hpp"
#include "physics/box2d_micrasbody.hpp"
#include "micras/proxy/proxy_bridge.hpp"
#include "render/render_engine.hpp"
#define GLFW_INCLUDE_VULKAN  // GLFW include guard
#include "GLFW/glfw3.h"

#include "vulkan_engine/first_app.hpp"
#include "vulkan_engine/simple_render_system.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

#include <vector>
#include <string>
#include <memory>
#include <chrono>

namespace micrasverse::render {

class GUI {
public:
    GUI(std::shared_ptr<lve::FirstApp> vulkanEngine, lve::SimpleRenderSystem& simpleRenderSystem);
    ~GUI() = default;

    void setSimulationEngine(const std::shared_ptr<micrasverse::simulation::SimulationEngine>& simulationEngine);
    void setRenderEngine(RenderEngine* renderEngine);
    void setProxyBridge(const std::shared_ptr<micras::ProxyBridge>& proxyBridge);
    void init(GLFWwindow* window);
    void update();
    void draw(micrasverse::physics::Box2DMicrasBody& micrasBody);
    void render();
    void destroy();

    ImGui_ImplVulkan_InitInfo getVulkanInitInfo(lve::FirstApp& app) const;

    bool isProxyBridgeInitialized() const { return proxyBridge != nullptr; }

private:
    bool                                                       showStyleEditor;
    GLFWwindow*                                                currentWindow;
    std::shared_ptr<micrasverse::simulation::SimulationEngine> simulationEngine;
    RenderEngine*                                              renderEngine;
    std::shared_ptr<micras::ProxyBridge>                       proxyBridge;
    std::shared_ptr<lve::FirstApp>                             vulkanEngine;
    lve::SimpleRenderSystem&                                   simpleRenderSystem;
    Plot                                                       plot;

    // Button timing variables
    bool                                  buttonTimerActive = false;
    std::chrono::steady_clock::time_point buttonActivationTime;
    float                                 buttonDurations[3] = {0.5f, 1.5f, 3.0f};  // Duration in seconds for SHORT, LONG, EXTRA_LONG
};

}  // namespace micrasverse::render

#endif  // GUI_HPP
