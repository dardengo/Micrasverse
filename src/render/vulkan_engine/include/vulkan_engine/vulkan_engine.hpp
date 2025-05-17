#ifndef LVE_VULKAN_ENGINE_HPP
#define LVE_VULKAN_ENGINE_HPP

#include "vulkan_engine/lve_device.hpp"
#include "vulkan_engine/lve_renderable_obj.hpp"
#include "vulkan_engine/lve_renderer.hpp"
#include "vulkan_engine/lve_window.hpp"
#include "constants.hpp"

#include <memory>
#include <vector>

namespace lve {
class VulkanEngine {
 public:
  VulkanEngine();
  ~VulkanEngine();

  VulkanEngine(const VulkanEngine &) = delete;
  VulkanEngine &operator=(const VulkanEngine &) = delete;

  void run();

 //private:
  void loadRenderableObjects();

  LveWindow lveWindow{micrasverse::WINDOW_WIDTH, micrasverse::WINDOW_HEIGHT, "Hello Vulkan!"};
  LveDevice lveDevice{lveWindow};
  LveRenderer lveRenderer{lveWindow, lveDevice};

  std::vector<LveRenderableObj> renderableObjects;
};
}  // namespace lve

#endif  // LVE_VULKAN_ENGINE_HPP
