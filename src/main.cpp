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
#include "vulkan_engine/first_app.hpp"
#include "vulkan_engine/simple_render_system.hpp"
#include "GUI/gui.hpp"
#include "vulkan_engine/lve_imgui.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

#include <iostream>
#include <memory>
#include <chrono>
#include <vector>

int main() {
    auto simulationEngine = std::make_shared<micrasverse::simulation::SimulationEngine>();

    auto& micrasBody = simulationEngine->physicsEngine->getMicras();
    micras::initializeProxyConfigs(&micrasBody);
    micras::Micras micrasController;
    auto           proxyBridge = std::make_shared<micras::ProxyBridge>(micrasController, micrasBody);

    auto vulkanEngine = std::make_shared<lve::FirstApp>(simulationEngine);
    vulkanEngine->setProxyBridge(proxyBridge);

    // auto renderEngine = std::make_shared<micrasverse::render::RenderEngine>(simulationEngine);
    // renderEngine->setProxyBridge(proxyBridge);
    // renderEngine->screen->setProxyBridge(proxyBridge);
    // renderEngine->screen->setRenderEngine(renderEngine.get());

    // Simple counter for controlling render frequency
    const int physicsStepsPerFrame = 40;
    int       stepCounter = 0;

    struct GlobalUbo {
        glm::mat4 projectionView{1.f};
    };

    // Initial render
    // renderEngine->update();
    // renderEngine->renderFrame();

    std::vector<std::unique_ptr<lve::LveBuffer>> uboBuffers(lve::LveSwapChain::MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < uboBuffers.size(); i++) {
        uboBuffers[i] = std::make_unique<lve::LveBuffer>(
            vulkanEngine->lveDevice, sizeof(GlobalUbo), 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
        );
        uboBuffers[i]->map();
    }

    auto globalSetLayout = lve::LveDescriptorSetLayout::Builder(vulkanEngine->lveDevice)
                               .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
                               .build();

    std::vector<VkDescriptorSet> globalDescriptorSets(lve::LveSwapChain::MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < globalDescriptorSets.size(); i++) {
        auto bufferInfo = uboBuffers[i]->descriptorInfo();
        lve::LveDescriptorWriter(*globalSetLayout, *vulkanEngine->globalPool).writeBuffer(0, &bufferInfo).build(globalDescriptorSets[i]);
    }

    lve::LveImgui lveImgui{
        vulkanEngine->lveWindow, vulkanEngine->lveDevice, vulkanEngine->lveRenderer.getSwapChainRenderPass(),
        vulkanEngine->lveRenderer.getImageCount()
    };

    lveImgui.setSimulationEngine(simulationEngine);
    lveImgui.setProxyBridge(proxyBridge);

    lve::SimpleRenderSystem simpleRenderSystem{
        vulkanEngine->lveDevice, vulkanEngine->lveRenderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()
    };

    auto camera = lve::LveCamera{};
    auto cameraController = lve::KeyboardMovementController{};
    auto viewerObject = lve::LveGameObject::createGameObject();
    auto currentTime = std::chrono::high_resolution_clock::now();
    viewerObject.transform.translation = {-0.25f, -micrasverse::MAZE_FLOOR_HALFHEIGHT, -4.0f};

    // micrasverse::render::GUI gui(vulkanEngine, simpleRenderSystem);

    // gui.setSimulationEngine(simulationEngine);
    // gui.init(vulkanEngine->lveWindow.window);

    // Main loop
    while (!glfwWindowShouldClose(vulkanEngine->lveWindow.window)) {
        glfwPollEvents();

        auto  newTime = std::chrono::high_resolution_clock::now();
        float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
        currentTime = newTime;

        cameraController.moveInPlaneXZ(vulkanEngine->lveWindow.getGLFWwindow(), frameTime, viewerObject);
        camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

        float aspect = vulkanEngine->lveRenderer.getAspectRatio();
        camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 1000.f);

        if (micrasverse::io::Keyboard::keyWentDown(GLFW_KEY_R)) {
            simulationEngine->togglePause();
        }

        if (!simulationEngine->isPaused) {
            micrasController.update();
            simulationEngine->updateSimulation();
            stepCounter++;
            if (stepCounter % physicsStepsPerFrame == 0) {
                // renderEngine->update();
                // renderEngine->renderFrame();
                vulkanEngine->updateRenderableModels();
                if (auto commandBuffer = vulkanEngine->lveRenderer.beginFrame()) {
                    lveImgui.newFrame();
                    int frameIndex = vulkanEngine->lveRenderer.getFrameIndex();

                    lve::FrameInfo frameInfo{frameIndex, frameTime, commandBuffer, camera, globalDescriptorSets[frameIndex]};

                    // update
                    GlobalUbo ubo{};
                    ubo.projectionView = camera.getProjection() * camera.getView();
                    uboBuffers[frameIndex]->writeToBuffer(&ubo);
                    uboBuffers[frameIndex]->flush();

                    // render
                    vulkanEngine->lveRenderer.beginSwapChainRenderPass(commandBuffer);
                    simpleRenderSystem.renderGameObjects(frameInfo, vulkanEngine->gameObjects);
                    lveImgui.runExample(micrasBody);
                    lveImgui.render(commandBuffer);
                    vulkanEngine->lveRenderer.endSwapChainRenderPass(commandBuffer);
                    vulkanEngine->lveRenderer.endFrame();
                }
            }
        } else {
            // renderEngine->update();
            // renderEngine->renderFrame();
            // vulkanEngine->drawFrame();
            vulkanEngine->updateRenderableModels();
            if (auto commandBuffer = vulkanEngine->lveRenderer.beginFrame()) {
                lveImgui.newFrame();
                int frameIndex = vulkanEngine->lveRenderer.getFrameIndex();

                lve::FrameInfo frameInfo{frameIndex, frameTime, commandBuffer, camera, globalDescriptorSets[frameIndex]};

                // update
                GlobalUbo ubo{};
                ubo.projectionView = camera.getProjection() * camera.getView();
                uboBuffers[frameIndex]->writeToBuffer(&ubo);
                uboBuffers[frameIndex]->flush();

                // render
                vulkanEngine->lveRenderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(frameInfo, vulkanEngine->gameObjects);
                lveImgui.runExample(micrasBody);
                lveImgui.render(commandBuffer);
                vulkanEngine->lveRenderer.endSwapChainRenderPass(commandBuffer);
                vulkanEngine->lveRenderer.endFrame();
            }
        }
    }
    vkDeviceWaitIdle(vulkanEngine->lveDevice.device());

    return 0;
}
