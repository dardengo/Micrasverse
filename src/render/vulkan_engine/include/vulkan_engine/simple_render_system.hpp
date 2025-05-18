#pragma once

#include "vulkan_engine/lve_camera.hpp"
#include "vulkan_engine/lve_device.hpp"
#include "vulkan_engine/lve_frame_info.hpp"
#include "vulkan_engine/lve_game_object.hpp"
#include "vulkan_engine/lve_pipeline.hpp"

// std
#include <memory>
#include <vector>

namespace lve {
class SimpleRenderSystem {
public:
    SimpleRenderSystem(LveDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
    ~SimpleRenderSystem();

    SimpleRenderSystem(const SimpleRenderSystem&) = delete;
    SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

    void renderGameObjects(FrameInfo& frameInfo, std::vector<LveGameObject>& gameObjects);

    // private:
    void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
    void createPipeline(VkRenderPass renderPass);

    LveDevice& lveDevice;

    std::unique_ptr<LvePipeline> lvePipeline;
    VkPipelineLayout             pipelineLayout;
};
}  // namespace lve
