#ifndef LVE_RENDERABLE_OBJ_HPP
#define LVE_RENDERABLE_OBJ_HPP

#include "vulkan_engine/lve_model.hpp"
#include <memory>
#include <glm/glm.hpp>

namespace lve {

struct Transform2dComponent {
  glm::vec2 translation{};
  glm::vec2 scale{1.0f, 1.0f};
  float rotation;

  glm::mat2 mat2() {
    const float s = glm::sin(rotation);
    const float c = glm::cos(rotation);
    glm::mat2 rotMatrix{{c, s}, {-s, c}};

    glm::mat2 scaleMat{{scale.x, 0.0f}, {0.0f, scale.y}};
    return rotMatrix * scaleMat;
  }
};

class LveRenderableObj {
 public:
  using id_t = unsigned int;

  static LveRenderableObj create() {
    static id_t currentId = 0;
    return LveRenderableObj{currentId++};
  }

  LveRenderableObj(const LveRenderableObj &) = delete;
  LveRenderableObj &operator=(const LveRenderableObj &) = delete;
  LveRenderableObj(LveRenderableObj &&) = default;
  LveRenderableObj &operator=(LveRenderableObj &&) = default;

  id_t getId() { return id; }

  std::shared_ptr<LveModel> model{};
  glm::vec3 color{};
  Transform2dComponent transform2d{};

 private:
  LveRenderableObj(id_t objId) : id{objId} {}

  id_t id;
};
}  // namespace lve

#endif // LVE_RENDERABLE_OBJ_HPP
