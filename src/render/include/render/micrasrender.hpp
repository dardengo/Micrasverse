#ifndef MICRAS_RENDER_HPP
#define MICRAS_RENDER_HPP

#include "render/sceneobj.hpp"

#include "box2d/box2d.h"

namespace micrasverse::render {

class MicrasRender: public SceneObj {
public:
    MicrasRender(const b2Vec2 position, const b2Vec2 size);

    void update(const b2Vec2 position, const b2Rot rotation);
};


} // namespace micrasverse::render

#endif // MICRAS_RENDER_HPP
