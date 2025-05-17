#ifndef LVE_RENDER_SYSTEM_HPP
#define LVE_RENDER_SYSTEM_HPP

#include "vulkan_engine/lve_device.hpp"
#include "vulkan_engine/lve_renderable_obj.hpp"
#include "vulkan_engine/lve_pipeline.hpp"

// std
#include <memory>
#include <vector>

namespace lve {
class LveRenderSystem {
 public:
  LveRenderSystem(LveDevice &device, VkRenderPass renderPass);
  ~LveRenderSystem();

  LveRenderSystem(const LveRenderSystem &) = delete;
  LveRenderSystem &operator=(const LveRenderSystem &) = delete;

  void renderRenderableObjects(VkCommandBuffer commandBuffer, std::vector<LveRenderableObj> &renderableObjects);

 private:
  void createPipelineLayout();
  void createPipeline(VkRenderPass renderPass);

  LveDevice &lveDevice;

  std::unique_ptr<LvePipeline> lvePipeline;
  VkPipelineLayout pipelineLayout;
};
}  // namespace lve

#endif  // LVE_RENDER_SYSTEM_HPP
