#include "vulkan_engine/vulkan_engine.hpp"
#include "vulkan_engine/lve_render_system.hpp"

#include <glm/gtc/constants.hpp>

#include <array>
#include <stdexcept>

namespace lve {

VulkanEngine::VulkanEngine() {loadRenderableObjects();}

VulkanEngine::~VulkanEngine() {}

void VulkanEngine::run() {
  
}

void VulkanEngine::loadRenderableObjects() {
  std::vector<LveModel::Vertex> vertices{
      {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
      {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
      {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};
  auto lveModel = std::make_shared<LveModel>(lveDevice, vertices);

  auto triangle = LveRenderableObj::create();
  triangle.model = lveModel;
  triangle.color = {.1f, .8f, .1f};
  triangle.transform2d.translation.x = .2f;
  triangle.transform2d.scale = {2.f, .5f};
  triangle.transform2d.rotation = .25f * glm::two_pi<float>();

  renderableObjects.push_back(std::move(triangle));
}

}  // namespace lve
