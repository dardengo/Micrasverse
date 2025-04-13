#ifndef LIDAR_RENDER_HPP
#define LIDAR_RENDER_HPP

#include "render/lightobj.hpp"
#include "render/camera.hpp"

#include "box2d/box2d.h"
#include "glm/glm.hpp"

namespace micrasverse::render {

class LidarRender: public LightObj {

public:
    glm::vec3 lightColor;
    Camera camera;
private:
    bool visible;  // Flag to show/hide the lidar beam

public:
    LidarRender(const b2Vec2 rayMidPoint, const b2Rot rayDirection, const float reading, const Camera& camera);
    
    void update(const b2Vec2 rayMidPoint, const b2Rot rayDirection, const float reading);

    void render(const glm::mat4 view, const glm::mat4 projection) override;

    bool isVisible() const { return visible; }
    void setVisible(bool value) { visible = value; }
};

} // namespace micrasverse::render


#endif // LIDAR_RENDER_HPP
