#pragma once

#include "vulkan_engine/lve_buffer.hpp"
#include "vulkan_engine/lve_device.hpp"

#include <vulkan/vulkan.h>

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

// std
#include <memory>
#include <vector>

namespace lve {
class LveModel {
public:
    struct Vertex {
        glm::vec3 position{};
        glm::vec3 color{};

        static std::vector<VkVertexInputBindingDescription>   getBindingDescriptions();
        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
    };

    struct Builder {
        std::vector<Vertex>   vertices{};
        std::vector<uint32_t> indices{};
    };

    LveModel(LveDevice& device, const LveModel::Builder& builder);
    ~LveModel();

    LveModel(const LveModel&) = delete;
    LveModel& operator=(const LveModel&) = delete;

    void bind(VkCommandBuffer commandBuffer);
    void draw(VkCommandBuffer commandBuffer);

private:
    void createVertexBuffers(const std::vector<Vertex>& vertices);
    void createIndexBuffers(const std::vector<uint32_t>& indices);

    LveDevice& lveDevice;

    std::unique_ptr<LveBuffer> vertexBuffer;
    uint32_t                   vertexCount;

    bool                       hasIndexBuffer = false;
    std::unique_ptr<LveBuffer> indexBuffer;
    uint32_t                   indexCount;
};
}  // namespace lve
