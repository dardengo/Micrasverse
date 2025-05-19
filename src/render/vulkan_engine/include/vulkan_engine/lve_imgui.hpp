#pragma once

#include "lve_device.hpp"
#include "lve_window.hpp"
#include "simulation/simulation_engine.hpp"
#include "GUI/plot.hpp"
#include "physics/box2d_micrasbody.hpp"

// libs
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

// std
#include <stdexcept>
#include <chrono>
#include <vector>
#include <memory>

// This whole class is only necessary right now because it needs to manage the descriptor pool
// because we haven't set one up anywhere else in the application, and we manage the
// example state, otherwise all the functions could just be static helper functions if you prefered
namespace lve {

static void check_vk_result(VkResult err) {
    if (err == 0)
        return;
    fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
    if (err < 0)
        abort();
}

class LveImgui {
public:
    LveImgui(LveWindow& window, LveDevice& device, VkRenderPass renderPass, uint32_t imageCount);
    ~LveImgui();

    void newFrame();

    void render(VkCommandBuffer commandBuffer);

    // Example state
    bool   show_demo_window = true;
    bool   show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    void   runExample(micrasverse::physics::Box2DMicrasBody& micrasBody);

    void setSimulationEngine(const std::shared_ptr<micrasverse::simulation::SimulationEngine>& simulationEngine);
    // void setRenderEngine(RenderEngine* renderEngine);
    void setProxyBridge(const std::shared_ptr<micras::ProxyBridge>& proxyBridge);
    void init(GLFWwindow* window);
    void update();
    void draw(micrasverse::physics::Box2DMicrasBody& micrasBody);
    void render();
    void destroy();

    bool isProxyBridgeInitialized() const { return proxyBridge != nullptr; }

    int physicsStepsPerFrame{40};

private:
    LveDevice&                                                 lveDevice;
    VkDescriptorPool                                           descriptorPool;
    bool                                                       showStyleEditor;
    GLFWwindow*                                                currentWindow;
    std::shared_ptr<micrasverse::simulation::SimulationEngine> simulationEngine;
    // RenderEngine*                                              renderEngine;
    std::shared_ptr<micras::ProxyBridge>  proxyBridge;
    micrasverse::render::Plot             plot;
    bool                                  buttonTimerActive = false;
    std::chrono::steady_clock::time_point buttonActivationTime;
    float                                 buttonDurations[3] = {0.5f, 1.5f, 3.0f};  // Duration in seconds for SHORT, LONG, EXTRA_LONG
};
}  // namespace lve
