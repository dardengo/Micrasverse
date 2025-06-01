#include "vulkan_engine/vulkan_engine.hpp"

#include "vulkan_engine/keyboard_movement_controller.hpp"
#include "vulkan_engine/lve_buffer.hpp"
#include "vulkan_engine/lve_camera.hpp"
#include "vulkan_engine/simple_render_system.hpp"
#include "physics/box2d_micrasbody.hpp"
#include "physics/box2d_distance_sensor.hpp"

#include "constants.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <cassert>
#include <chrono>
#include <stdexcept>
#include <cmath>

namespace lve {

struct GlobalUbo {
    glm::mat4 projectionView{1.f};
};

VulkanEngine::VulkanEngine(std::shared_ptr<micrasverse::simulation::SimulationEngine> engine) : simulationEngine(engine) {
    globalPool = LveDescriptorPool::Builder(lveDevice)
                     .setMaxSets(LveSwapChain::MAX_FRAMES_IN_FLIGHT)
                     .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, LveSwapChain::MAX_FRAMES_IN_FLIGHT)
                     .build();
    loadGameObjects();
}

VulkanEngine::~VulkanEngine() { }

void VulkanEngine::init() {
    // std::vector<std::unique_ptr<LveBuffer>> uboBuffers(LveSwapChain::MAX_FRAMES_IN_FLIGHT);
    // for (int i = 0; i < uboBuffers.size(); i++) {
    //   uboBuffers[i] = std::make_unique<LveBuffer>(
    //       lveDevice,
    //       sizeof(GlobalUbo),
    //       1,
    //       VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
    //       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
    //   uboBuffers[i]->map();
    // }

    // auto globalSetLayout =
    //     LveDescriptorSetLayout::Builder(lveDevice)
    //         .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
    //         .build();

    // std::vector<VkDescriptorSet> globalDescriptorSets(LveSwapChain::MAX_FRAMES_IN_FLIGHT);
    // for (int i = 0; i < globalDescriptorSets.size(); i++) {
    //   auto bufferInfo = uboBuffers[i]->descriptorInfo();
    //   LveDescriptorWriter(*globalSetLayout, *globalPool)
    //       .writeBuffer(0, &bufferInfo)
    //       .build(globalDescriptorSets[i]);
    // }

    // SimpleRenderSystem simpleRenderSystem{
    //   lveDevice,
    //   lveRenderer.getSwapChainRenderPass(),
    //   globalSetLayout->getDescriptorSetLayout()
    // };

    // this->camera = LveCamera{};
    // this->cameraController = KeyboardMovementController{};
    // this->viewerObject = LveGameObject::createGameObject();
    // this->currentTime = std::chrono::high_resolution_clock::now();
}

void VulkanEngine::run() {
    // while (!lveWindow.shouldClose()) {
    //   glfwPollEvents();

    //   auto newTime = std::chrono::high_resolution_clock::now();
    //   float frameTime =
    //       std::chrono::duration<float, std::chrono::seconds::period>(newTime - this->currentTime).count();
    //   this->currentTime = newTime;

    //   this->cameraController.moveInPlaneXZ(lveWindow.getGLFWwindow(), frameTime, this->viewerObject);
    //   this->camera.setViewYXZ(this->viewerObject.transform.translation, this->viewerObject.transform.rotation);

    //   float aspect = lveRenderer.getAspectRatio();
    //   this->camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 1000.f);

    //   if (auto commandBuffer = lveRenderer.beginFrame()) {
    //     int frameIndex = lveRenderer.getFrameIndex();
    //     FrameInfo frameInfo{
    //         frameIndex,
    //         frameTime,
    //         commandBuffer,
    //         this->camera,
    //         globalDescriptorSets[frameIndex]};

    //     // update
    //     GlobalUbo ubo{};
    //     ubo.projectionView = this->camera.getProjection() * this->camera.getView();
    //     uboBuffers[frameIndex]->writeToBuffer(&ubo);
    //     uboBuffers[frameIndex]->flush();

    //     // render
    //     lveRenderer.beginSwapChainRenderPass(commandBuffer);
    //     simpleRenderSystem.renderGameObjects(frameInfo, this->gameObjects);
    //     lveRenderer.endSwapChainRenderPass(commandBuffer);
    //     lveRenderer.endFrame();
    //   }
    // }

    // vkDeviceWaitIdle(lveDevice.device());
}

std::unique_ptr<LveModel> createCubeModel(LveDevice& device, glm::vec3 offset) {
    LveModel::Builder modelBuilder{};
    modelBuilder.vertices = {
        // left face (white)
        {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
        {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
        {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
        {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},

        // right face (yellow)
        {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
        {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
        {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
        {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},

        // top face (orange, remember y axis points down)
        {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
        {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
        {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
        {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},

        // bottom face (red)
        {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
        {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
        {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
        {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},

        // nose face (blue)
        {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
        {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
        {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
        {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},

        // tail face (green)
        {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
        {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
        {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
        {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
    };
    for (auto& v : modelBuilder.vertices) {
        v.position += offset;
    }

    modelBuilder.indices = {0,  1,  2,  0,  3,  1,  4,  5,  6,  4,  7,  5,  8,  9,  10, 8,  11, 9,
                            12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21};

    return std::make_unique<LveModel>(device, modelBuilder);
}

std::unique_ptr<LveModel> createRectModel(LveDevice& device, glm::vec3 offset, glm::vec3 color) {
    LveModel::Builder modelBuilder{};
    modelBuilder.vertices = {
        {{-.5f, -.5f, 0.5f}, color},
        {{.5f, .5f, 0.5f}, color},
        {{-.5f, .5f, 0.5f}, color},
        {{.5f, -.5f, 0.5f}, color},
    };
    for (auto& v : modelBuilder.vertices) {
        v.position += offset;
    }

    modelBuilder.indices = {0, 1, 2, 0, 3, 1};

    return std::make_unique<LveModel>(device, modelBuilder);
}

std::unique_ptr<LveModel> createTriangleModel(LveDevice& device, glm::vec3 offset, glm::vec3 color) {
    LveModel::Builder modelBuilder{};
    modelBuilder.vertices = {
        {{-.5f, .5f, 0.5f}, color},
        {{.5f, .5f, 0.5f}, color},
        {{.0f, -.5f, 0.5f}, color},
    };
    for (auto& v : modelBuilder.vertices) {
        v.position += offset;
    }

    modelBuilder.indices = {0, 1, 2};

    return std::make_unique<LveModel>(device, modelBuilder);
}

void VulkanEngine::loadGameObjects() {
    // std::shared_ptr<LveModel> lveModel = createCubeModel(lveDevice, {.0f, .0f, .0f});
    // auto cube = LveGameObject::createGameObject();
    // cube.model = lveModel;
    // cube.transform.translation = {.0f, .0f, 2.5f};
    // cube.transform.scale = {.5f, .5f, .5f};
    // gameObjects.push_back(std::move(cube));

    loadLidar();
    loadARGB();
    loadMicras();
    loadMazeWalls();
    loadMazeFloor();
}

void VulkanEngine::loadMazeFloor() {
    std::shared_ptr<LveModel> lveModel = createRectModel(lveDevice, {.0f, .0f, .0f}, {0.f, 0.f, 0.f});
    auto                      floor = LveGameObject::createGameObject();
    floor.model = lveModel;
    floor.transform.translation = {micrasverse::MAZE_FLOOR_HALFWIDTH, -micrasverse::MAZE_FLOOR_HALFHEIGHT, 0.0f};
    floor.transform.scale = {micrasverse::MAZE_FLOOR_WIDTH, micrasverse::MAZE_FLOOR_HEIGHT, 0.0f};
    gameObjects.push_back(std::move(floor));
}

void VulkanEngine::loadMazeWalls() {
    this->first_wall_index = gameObjects.size();

    for (auto& wall : simulationEngine->physicsEngine->getMaze().getElements()) {
        std::shared_ptr<LveModel> lveModel = createRectModel(lveDevice, {.0f, .0f, .0f}, {0.5f, 0.0f, 0.0f});
        auto                      wallObject = LveGameObject::createGameObject();
        wallObject.model = lveModel;
        wallObject.transform.translation = glm::vec3(wall.position.x, -wall.position.y, 0.0f);
        wallObject.transform.scale = glm::vec3(wall.size.x, wall.size.y, 0.f);
        this->number_of_walls++;
        gameObjects.push_back(std::move(wallObject));
    }
}

void VulkanEngine::reloadMazeWalls() {
    // remove walls from game objects
    gameObjects.erase(gameObjects.begin() + first_wall_index, gameObjects.begin() + number_of_walls + first_wall_index);
    // run loadMazeWalls
    this->number_of_walls = 0;
    this->walls_set.clear();

    this->loadMazeWalls();
    this->loadMazeFloor();
}

void VulkanEngine::loadFirmwareMazeWalls() {
    if (!proxyBridge) {
        return;
    }

    for (uint8_t y = 0; y < micras::maze_height; y++) {
        for (uint8_t x = 0; x < micras::maze_width; x++) {
            const micras::nav::GridPoint point{x, y};

            for (uint8_t dir = 0; dir < 4; dir++) {
                micras::nav::GridPose                      pose{point, static_cast<micras::nav::Side>(dir)};
                micras::nav::Costmap<16, 16, 2>::WallState wallState = proxyBridge->get_wall_state(pose);

                if (this->walls_set.contains(pose)) {
                    continue;
                }

                if (wallState == micras::nav::Costmap<16, 16, 2>::WallState::WALL or
                    wallState == micras::nav::Costmap<16, 16, 2>::WallState::VIRTUAL) {
                    walls_set.insert(pose);

                    float posX = 0.0f, posY = 0.0f;
                    float sizeX = 0.0f, sizeY = 0.0f;

                    switch (dir) {
                        case micras::nav::Side::RIGHT:  // East (0)
                            posX = (x + 1) * micrasverse::CELL_SIZE + (micrasverse::WALL_THICKNESS / 2.0f);
                            posY = y * micrasverse::CELL_SIZE + (micrasverse::CELL_SIZE + micrasverse::WALL_THICKNESS) / 2.0f;
                            sizeX = micrasverse::WALL_THICKNESS;
                            sizeY = micrasverse::WALL_SIZE;
                            break;

                        case micras::nav::Side::UP:  // North (1)
                            posX = x * micrasverse::CELL_SIZE + (micrasverse::CELL_SIZE + micrasverse::WALL_THICKNESS) / 2.0f;
                            posY = (y + 1) * micrasverse::CELL_SIZE + (micrasverse::WALL_THICKNESS / 2.0f);
                            sizeX = micrasverse::WALL_SIZE;
                            sizeY = micrasverse::WALL_THICKNESS;
                            break;

                        case micras::nav::Side::LEFT:  // West (2)
                            posX = x * micrasverse::CELL_SIZE + micrasverse::WALL_THICKNESS / 2.0f;
                            posY = y * micrasverse::CELL_SIZE + (micrasverse::CELL_SIZE + micrasverse::WALL_THICKNESS) / 2.0f;
                            sizeX = micrasverse::WALL_THICKNESS;
                            sizeY = micrasverse::WALL_SIZE;
                            break;

                        case micras::nav::Side::DOWN:  // South (3)
                            posX = x * micrasverse::CELL_SIZE + (micrasverse::CELL_SIZE + micrasverse::WALL_THICKNESS) / 2.0f;
                            posY = y * micrasverse::CELL_SIZE + (micrasverse::WALL_THICKNESS / 2.0f);
                            sizeX = micrasverse::WALL_SIZE;
                            sizeY = micrasverse::WALL_THICKNESS;
                            break;
                    }

                    auto wallState = proxyBridge->get_cell(point).walls[dir];

                    glm::vec3 color;
                    switch (wallState) {
                        case micras::nav::Costmap<16, 16, 2>::WallState::WALL:
                            color = glm::vec3(0.0f, 1.0f, 1.0f);
                            break;
                        case micras::nav::Costmap<16, 16, 2>::WallState::VIRTUAL:
                            color = glm::vec3(1.0f, 1.0f, 0.0f);
                            break;
                    }

                    std::shared_ptr<LveModel> lveModel = createRectModel(lveDevice, {.0f, .0f, .0f}, color);
                    auto                      wallObject = LveGameObject::createGameObject();
                    wallObject.model = lveModel;
                    wallObject.transform.translation = glm::vec3(posX, -posY, -0.00001f);
                    wallObject.transform.scale = glm::vec3(sizeX, sizeY, 0.0f);
                    number_of_walls++;
                    gameObjects.push_back(std::move(wallObject));
                }
            }
        }
    }
}

void VulkanEngine::loadBestRoute() {
    if (!proxyBridge) {
        return;
    }

    gameObjects.erase(gameObjects.begin() + first_route_marker_index, gameObjects.begin() + number_of_route_markers + first_route_marker_index);
    this->number_of_route_markers = 0;
    this->best_route_set.clear();

    this->first_route_marker_index = gameObjects.size();

    const auto& best_route = proxyBridge->get_best_route();

    for (const auto& grid_point : best_route) {
        if (this->best_route_set.contains(grid_point)) {
            continue;
        }
        this->best_route_set.insert(grid_point);
        float posX = grid_point.x * micrasverse::CELL_SIZE + (micrasverse::CELL_SIZE / 2.0f) + micrasverse::WALL_THICKNESS / 2.0f;
        float posY = grid_point.y * micrasverse::CELL_SIZE + (micrasverse::CELL_SIZE / 2.0f) + micrasverse::WALL_THICKNESS / 2.0f;

        std::shared_ptr<LveModel> lveModel = createRectModel(lveDevice, {.0f, .0f, .0f}, {0.0f, 1.0f, 0.0f});
        auto                      wallObject = LveGameObject::createGameObject();
        wallObject.model = lveModel;
        wallObject.transform.translation = glm::vec3(posX, -posY, -0.00001f);
        wallObject.transform.scale = glm::vec3(0.03f, 0.03f, 0.0f);
        this->number_of_route_markers++;
        gameObjects.push_back(std::move(wallObject));
    }
}

void VulkanEngine::loadMicras() {
    std::shared_ptr<LveModel> lveModel = createRectModel(lveDevice, {.0f, .0f, .0f}, {0.0f, 0.5f, 0.0f});
    auto                      micras = LveGameObject::createGameObject();
    micras.model = lveModel;
    micras.transform.translation =
        glm::vec3(simulationEngine->physicsEngine->getMicras().getPosition().x, -simulationEngine->physicsEngine->getMicras().getPosition().y, 0.0f);
    micras.transform.scale =
        glm::vec3(simulationEngine->physicsEngine->getMicras().getSize().x, simulationEngine->physicsEngine->getMicras().getSize().y, 0.f);
    this->micrasIndex = gameObjects.size();
    gameObjects.push_back(std::move(micras));
}

void VulkanEngine::loadARGB() {
    // for (auto& led : simulationEngine->physicsEngine->getMicras().getArgbs()) {
    //     std::shared_ptr<LveModel> lveModel = createRectModel(lveDevice, {.0f, .0f, .0f}, {0.3f, 0.3f, 0.3f});
    //     auto                      ledObject = LveGameObject::createGameObject();
    //     ledObject.model = lveModel;
    //     ledObject.transform.translation = glm::vec3(led->worldPosition.x, -led->worldPosition.y, 0.0f);
    //     ledObject.transform.scale = glm::vec3(led->size.x, led->size.y, 0.f);
    //     gameObjects.push_back(std::move(ledObject));
    // }
    // this->argbIndex = gameObjects.size();
}

void VulkanEngine::loadLidar() {
    for (size_t i = 0; i < simulationEngine->physicsEngine->getMicras().getDistanceSensorCount(); i++) {
        std::shared_ptr<LveModel> lveModel = createRectModel(lveDevice, {.0f, .0f, .0f}, {1.0f, 0.1f, 0.1f});
        auto                      lidarObject = LveGameObject::createGameObject();
        lidarObject.model = lveModel;
        lidarObject.transform.translation = glm::vec3(
            simulationEngine->physicsEngine->getMicras().getDistanceSensor(i).getVisualMidPoint().x,
            -simulationEngine->physicsEngine->getMicras().getDistanceSensor(i).getVisualMidPoint().y, 0.0f
        );
        lidarObject.transform.scale = glm::vec3(0.005f, simulationEngine->physicsEngine->getMicras().getDistanceSensor(i).getReadingVisual(), 0.0f);
        gameObjects.push_back(std::move(lidarObject));
    }
    this->lidarIndex = gameObjects.size();
}

void VulkanEngine::updateRenderableModels() {
    auto& micras = gameObjects[micrasIndex];
    micras.transform.translation =
        glm::vec3(simulationEngine->physicsEngine->getMicras().getPosition().x, -simulationEngine->physicsEngine->getMicras().getPosition().y, 0.0f);
    micras.transform.rotation = glm::vec3(0.f, 0.f, -simulationEngine->physicsEngine->getMicras().getAngle());

    // for (size_t i = 0; i < simulationEngine->physicsEngine->getMicras().getArgbs().size(); i++) {
    //     auto& argb = gameObjects[argbIndex - simulationEngine->physicsEngine->getMicras().getArgbs().size() + i];
    //     argb.transform.translation = glm::vec3(
    //         simulationEngine->physicsEngine->getMicras().getArgbs()[i]->worldPosition.x,
    //         -simulationEngine->physicsEngine->getMicras().getArgbs()[i]->worldPosition.y, 0.0f
    //     );
    //     argb.transform.rotation = glm::vec3(0.f, 0.f, -simulationEngine->physicsEngine->getMicras().getAngle());
    // }

    for (size_t i = 0; i < simulationEngine->physicsEngine->getMicras().getDistanceSensorCount(); i++) {
        auto& sensor = gameObjects[lidarIndex - simulationEngine->physicsEngine->getMicras().getDistanceSensorCount() + i];
        sensor.transform.translation = glm::vec3(
            simulationEngine->physicsEngine->getMicras().getDistanceSensor(i).getVisualMidPoint().x,
            -simulationEngine->physicsEngine->getMicras().getDistanceSensor(i).getVisualMidPoint().y, 0.0f
        );
        float angle = std::atan2(
            simulationEngine->physicsEngine->getMicras().getDistanceSensor(i).getRayDirection().y,
            simulationEngine->physicsEngine->getMicras().getDistanceSensor(i).getRayDirection().x
        );
        sensor.transform.rotation = glm::vec3(0.f, 0.f, -angle);
        sensor.transform.scale = glm::vec3(0.005f, simulationEngine->physicsEngine->getMicras().getDistanceSensor(i).getReadingVisual(), 0.0f);
    }

    loadFirmwareMazeWalls();

    micras::core::Objective currentObjective = proxyBridge->get_objective();
    bool                    objectiveChanged = (currentObjective != lastObjective);
    lastObjective = currentObjective;

    if (objectiveChanged) {
        loadBestRoute();
    }

    if (this->simulationEngine->wasReset) {
        this->reloadMazeWalls();
        this->simulationEngine->wasReset = false;
    }
}

}  // namespace lve
