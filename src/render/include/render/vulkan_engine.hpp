#ifndef VULKAN_ENGINE_HPP
#define VULKAN_ENGINE_HPP

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <optional>
#include <fstream>
#include <glm/glm.hpp>
#include "constants.hpp"

namespace micrasverse::render {

class VulkanEngine {
public:
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() { return graphicsFamily.has_value(); }
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR        capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR>   presentModes;
    };

    void initWindow();

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

    void createInstance();

    void pickPhysicalDevice();

    int rateDeviceSuitability(VkPhysicalDevice device);

    bool isDeviceSuitable(VkPhysicalDevice device);

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    void createLogicalDevice();

    void createSurface();

    bool checkDeviceExtensionSupport(VkPhysicalDevice device);

    void createSwapChain();

    void cleanupSwapChain();

    void recreateSwapChain();

    void createImageViews();

    std::vector<char> readFile(const std::string& filename);

    VkShaderModule createShaderModule(const std::vector<char>& code);

    void createRenderPass();

    void createDescriptorSetLayout();

    void createGraphicsPipeline();

    void createFramebuffers();

    void createCommandPool();

    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    void createCommandBuffers();

    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, const std::vector<VkDescriptorSet>& descriptorSets);

    void createSyncObjects();

    void initVulkan();

    void drawFrame();

    void mainLoop();

    void cleanup();

    bool checkValidationLayerSupport();

    GLFWwindow* window;

    VkDevice device;

    VkRenderPass                 renderPass;
    VkExtent2D                   swapChainExtent;
    bool                         framebufferResized = false;
//private:
    const uint32_t               WIDTH = WINDOW_WIDTH;
    const uint32_t               HEIGHT = WINDOW_HEIGHT;
    VkInstance                   instance;
    VkPhysicalDevice             physicalDevice = VK_NULL_HANDLE;
    VkQueue                      graphicsQueue;
    VkQueue                      presentQueue;
    VkSurfaceKHR                 surface;
    VkSwapchainKHR               swapChain;
    std::vector<VkImage>         swapChainImages;
    VkFormat                     swapChainImageFormat;
    std::vector<VkImageView>     swapChainImageViews;
    VkShaderModule               vertShaderModule;
    VkShaderModule               fragShaderModule;
    VkDescriptorSetLayout        descriptorSetLayout;
    VkPipelineLayout             pipelineLayout;
    VkPipeline                   graphicsPipeline;
    std::vector<VkFramebuffer>   swapChainFramebuffers;
    VkCommandPool                commandPool;
    std::vector<VkCommandBuffer> commandBuffers;
    std::vector<VkSemaphore>     imageAvailableSemaphores;
    std::vector<VkSemaphore>     renderFinishedSemaphores;
    std::vector<VkFence>         inFlightFences;
    uint32_t                     currentFrame = 0;
    VkDescriptorPool             descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;

    const std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif
};

}  // namespace micrasverse::render

#endif  // VULKAN_ENGINE_HPP
