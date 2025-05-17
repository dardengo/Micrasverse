#include "constants.hpp"
#include "simulation/simulation_engine.hpp"
#include "render/render_engine.hpp"
#include "micras/micras.hpp"
#include "micras/proxy/proxy_bridge.hpp"
#include "target.hpp"
#include "box2d/box2d.h"
#include "io/keyboard.hpp"
#include "models/rectangle.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "vulkan_engine/vulkan_engine.hpp"
#include "vulkan_engine/lve_render_system.hpp"

#include <iostream>

int main() {
    auto simulationEngine = std::make_shared<micrasverse::simulation::SimulationEngine>();

    auto& micrasBody = simulationEngine->physicsEngine->getMicras();
    micras::initializeProxyConfigs(&micrasBody);
    micras::Micras micrasController;
    auto proxyBridge = std::make_shared<micras::ProxyBridge>(micrasController, micrasBody);

    auto vulkanEngine = std::make_shared<lve::VulkanEngine>();
    auto renderSystem = std::make_shared<lve::LveRenderSystem>(vulkanEngine->lveDevice, vulkanEngine->lveRenderer.getSwapChainRenderPass());

    //auto renderEngine = std::make_shared<micrasverse::render::RenderEngine>(simulationEngine);
    //renderEngine->setProxyBridge(proxyBridge);
    //renderEngine->screen->setProxyBridge(proxyBridge);
    //renderEngine->screen->setRenderEngine(renderEngine.get());

    // Simple counter for controlling render frequency
    const int physicsStepsPerFrame = 20;
    int stepCounter = 0;
    
    
    // Initial render
    //renderEngine->update();
    //renderEngine->renderFrame();

    // Main loop
    while (/*not renderEngine->screen->shouldClose()*/ !glfwWindowShouldClose(vulkanEngine->lveWindow.window)) {

        glfwPollEvents();
        if (micrasverse::io::Keyboard::keyWentDown(GLFW_KEY_R)) {
            simulationEngine->togglePause();
        }

        if (/*!simulationEngine->isPaused*/ true) {
            micrasController.update();
            simulationEngine->updateSimulation();
            stepCounter++;
            std::cerr << "Step: " << stepCounter << '\n';
            if (stepCounter % physicsStepsPerFrame == 0) {
                //renderEngine->update();
                //renderEngine->renderFrame();
                if (auto commandBuffer = vulkanEngine->lveRenderer.beginFrame()) {
                    vulkanEngine->lveRenderer.beginSwapChainRenderPass(commandBuffer);
                    renderSystem->renderRenderableObjects(commandBuffer, vulkanEngine->renderableObjects);
                    vulkanEngine->lveRenderer.endSwapChainRenderPass(commandBuffer);
                    vulkanEngine->lveRenderer.endFrame();
                }
            }
        } else {
            //renderEngine->update();
            //renderEngine->renderFrame();
            //vulkanEngine->drawFrame();
        }
        
    }
    vkDeviceWaitIdle(vulkanEngine->lveDevice.device());

    
    return 0;
}
