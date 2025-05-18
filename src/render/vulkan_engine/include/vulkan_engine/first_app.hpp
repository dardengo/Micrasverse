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

#include <memory>
#include <vector>
#include <chrono>

namespace lve {
class FirstApp {
public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    FirstApp(std::shared_ptr<micrasverse::simulation::SimulationEngine> engine);
    ~FirstApp();

    FirstApp(const FirstApp&) = delete;
    FirstApp& operator=(const FirstApp&) = delete;

    void init();
    void run();

    // private:
    void loadGameObjects();
    void loadMazeFloor();
    void loadMazeWalls();
    void loadFirmwareMazeWalls();
    void loadMicras();
    void loadARGB();
    void loadLidar();

    void updateRenderableModels();

    LveWindow                                                  lveWindow{WIDTH, HEIGHT, "Vulkan Tutorial"};
    LveDevice                                                  lveDevice{lveWindow};
    LveRenderer                                                lveRenderer{lveWindow, lveDevice};
    std::shared_ptr<micrasverse::simulation::SimulationEngine> simulationEngine;
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
};
}  // namespace lve
