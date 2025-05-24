#pragma once

#include "vulkan_engine/lve_descriptors.hpp"
#include "vulkan_engine/lve_device.hpp"
#include "vulkan_engine/lve_game_object.hpp"
#include "vulkan_engine/lve_renderer.hpp"
#include "vulkan_engine/lve_window.hpp"
#include "simulation/simulation_engine.hpp"
#include "vulkan_engine/lve_camera.hpp"
#include "vulkan_engine/keyboard_movement_controller.hpp"
#include "vulkan_engine/simple_render_system.hpp"
#include "micras/proxy/proxy_bridge.hpp"
#include "micras/core/types.hpp"

#include <memory>
#include <vector>
#include <chrono>
#include <unordered_set>

namespace lve {
class VulkanEngine {
public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    VulkanEngine(std::shared_ptr<micrasverse::simulation::SimulationEngine> engine);
    ~VulkanEngine();

    VulkanEngine(const VulkanEngine&) = delete;
    VulkanEngine& operator=(const VulkanEngine&) = delete;

    void init();
    void run();

    // private:
    void loadGameObjects();
    void loadMazeFloor();
    void loadMazeWalls();
    void reloadMazeWalls();
    void loadFirmwareMazeWalls();
    void loadBestRoute();
    void loadMicras();
    void loadARGB();
    void loadLidar();

    void updateRenderableModels();

    void setProxyBridge(std::shared_ptr<micras::ProxyBridge> proxyBridge) { this->proxyBridge = proxyBridge; }

    LveWindow                                                  lveWindow{WIDTH, HEIGHT, "Micrasverse"};
    LveDevice                                                  lveDevice{lveWindow};
    LveRenderer                                                lveRenderer{lveWindow, lveDevice};
    std::shared_ptr<micrasverse::simulation::SimulationEngine> simulationEngine;
    std::shared_ptr<micras::ProxyBridge>                       proxyBridge;
    // note: order of declarations matters
    std::unique_ptr<LveDescriptorPool> globalPool{};
    std::vector<LveGameObject>         gameObjects;

    LveCamera                                                   camera;
    KeyboardMovementController                                  cameraController;
    LveGameObject                                               viewerObject;
    std::chrono::time_point<std::chrono::high_resolution_clock> currentTime;
    uint16_t                                                    micrasIndex{0};
    uint16_t                                                    lidarIndex{0};
    uint16_t                                                    argbIndex{0};
    bool                                                        shouldClose{false};

    std::unordered_set<micras::nav::GridPose> walls_set;
    std::unordered_set<micras::nav::GridPose> best_route_set;

    int first_wall_index{0};
    int number_of_walls{0};

    int                     first_route_marker_index{0};
    int                     number_of_route_markers{0};
    micras::core::Objective lastObjective = micras::core::Objective::EXPLORE;
};
}  // namespace lve
