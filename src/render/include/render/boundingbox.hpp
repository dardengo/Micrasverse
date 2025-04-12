#ifndef BOUNDINGBOX_HPP
#define BOUNDINGBOX_HPP

#include "glm/glm.hpp"

namespace micrasverse::render {

// Simple AABB for frustum culling
struct BoundingBox {
    glm::vec3 min;
    glm::vec3 max;
    
    BoundingBox(glm::vec3 min = glm::vec3(-1.0f), glm::vec3 max = glm::vec3(1.0f))
        : min(min), max(max) {}
};

} // namespace micrasverse::render

#endif // BOUNDINGBOX_HPP 
