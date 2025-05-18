#pragma once

#include "vulkan_engine/lve_game_object.hpp"
#include "vulkan_engine/lve_window.hpp"

namespace lve {
class KeyboardMovementController {
public:
    struct KeyMappings {
        int moveLeft = GLFW_KEY_LEFT;
        int moveRight = GLFW_KEY_RIGHT;
        int moveUp = GLFW_KEY_UP;
        int moveDown = GLFW_KEY_DOWN;
    };

    void moveInPlaneXZ(GLFWwindow* window, float dt, LveGameObject& gameObject);

    KeyMappings keys{};
    float       moveSpeed{3.f};
    float       lookSpeed{1.5f};
};
}  // namespace lve
