#ifndef VULKAN_ENGINE_HPP
#define VULKAN_ENGINE_HPP

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <optional>
#include <fstream>
#include <glm/glm.hpp>

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

    struct Vertex {
        glm::vec2 position;
        glm::vec3 color;

        static VkVertexInputBindingDescription getBindingDescription() {
            VkVertexInputBindingDescription bindingDescription = {};
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(Vertex);
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            return bindingDescription;
        }

        static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
            std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {};
            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(Vertex, position);

            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[1].offset = offsetof(Vertex, color);

            return attributeDescriptions;
        }
    };

    struct UniformBufferObject {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    };

    void run();

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

    void createVertexBuffer();

    void createIndexBuffer();

    void createUniformBuffers();

    void createDescriptorPool();

    void createDescriptorSets();

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    void createCommandBuffers();

    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

    void createSyncObjects();

    void init();

    void drawFrame();

    void updateUniformBuffer(uint32_t currentImage);

    void mainLoop();

    void cleanup();

    bool checkValidationLayerSupport();

    GLFWwindow* window;

    VkDevice device;

    VkRenderPass                 renderPass;
    VkExtent2D                   swapChainExtent;
    bool                         framebufferResized = false;
private:
    const uint32_t               WIDTH = 800;
    const uint32_t               HEIGHT = 600;
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
    VkBuffer                     vertexBuffer;
    VkDeviceMemory               vertexBufferMemory;
    VkBuffer                     indexBuffer;
    VkDeviceMemory               indexBufferMemory;
    std::vector<VkBuffer>        uniformBuffers;
    std::vector<VkDeviceMemory>  uniformBuffersMemory;
    std::vector<void*>           uniformBuffersMapped;
    VkDescriptorPool             descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;

    const std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};

    const std::vector<Vertex> vertices = {
        {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
    };

    const std::vector<uint16_t> indices = {0, 1, 2, 2, 3, 0};

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif
};

}  // namespace micrasverse::render

#endif  // VULKAN_ENGINE_HPP
