#ifndef LIDAR_RENDER_HPP
#define LIDAR_RENDER_HPP

#include "render/sceneobj.hpp"

#include "box2d/box2d.h"

namespace micrasverse::render {

class LidarRender: public SceneObj {

public:
    LidarRender(const b2Vec2 rayMidPoint, const b2Rot rayDirection, const float reading);
    
    void update(const b2Vec2 rayMidPoint, const b2Rot rayDirection, const float reading);
};

} // namespace micrasverse::render


#endif // LIDAR_RENDER_HPP
