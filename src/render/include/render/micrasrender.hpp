#ifndef MICRAS_RENDER_HPP
#define MICRAS_RENDER_HPP

#include "render/sceneobj.hpp"
#include "render/boundingbox.hpp"

#include "box2d/box2d.h"

namespace micrasverse::render {

class MicrasRender: public SceneObj {
public:
    MicrasRender(const b2Vec2 position, const b2Vec2 size);

    void update(const b2Vec2 position, const b2Rot rotation);
    
    // Get AABB for frustum culling
    BoundingBox getBoundingBox() const;
    
private:
    b2Vec2 position;
    b2Vec2 size;
};


} // namespace micrasverse::render

#endif // MICRAS_RENDER_HPP
