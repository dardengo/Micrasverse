#pragma once

#include "vulkan_engine/lve_device.hpp"
#include "vulkan_engine/lve_swap_chain.hpp"
#include "vulkan_engine/lve_window.hpp"

// std
#include <cassert>
#include <memory>
#include <vector>

namespace lve {
class LveRenderer {
public:
    LveRenderer(LveWindow& window, LveDevice& device);
    ~LveRenderer();

    LveRenderer(const LveRenderer&) = delete;
    LveRenderer& operator=(const LveRenderer&) = delete;

    VkRenderPass getSwapChainRenderPass() const { return lveSwapChain->getRenderPass(); }

    uint32_t getImageCount() const { return lveSwapChain->imageCount(); }

    float getAspectRatio() const { return lveSwapChain->extentAspectRatio(); }

    bool isFrameInProgress() const { return isFrameStarted; }

    VkCommandBuffer getCurrentCommandBuffer() const {
        assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
        return commandBuffers[currentFrameIndex];
    }

    int getFrameIndex() const {
        assert(isFrameStarted && "Cannot get frame index when frame not in progress");
        return currentFrameIndex;
    }

    VkCommandBuffer beginFrame();
    void            endFrame();
    void            beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
    void            endSwapChainRenderPass(VkCommandBuffer commandBuffer);

    //  private:
    void createCommandBuffers();
    void freeCommandBuffers();
    void recreateSwapChain();

    LveWindow&                    lveWindow;
    LveDevice&                    lveDevice;
    std::unique_ptr<LveSwapChain> lveSwapChain;
    std::vector<VkCommandBuffer>  commandBuffers;

    uint32_t currentImageIndex;
    int      currentFrameIndex{0};
    bool     isFrameStarted{false};
};
}  // namespace lve
