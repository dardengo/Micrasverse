#ifndef PLOT_HPP
#define PLOT_HPP

#include "physics/micrasbody.hpp"

#include "imgui.h"

namespace micrasverse::render {

class Plot {
public:
    struct ScrollingBuffer {
        int maxSize;
        int offset;
        ImVector<ImVec2> data;
        
        ScrollingBuffer(int maxSize = 2000);
        
        void addPoint(const float x, const float y);
        void erase();
    };

    struct RollingBuffer{
        int span;
        ImVector<ImVec2> data;

        RollingBuffer(int span = 10, int maxSize = 2000);
        
        void addPoint(const float x, const float y);
    };
    
    Plot();

    void init();

    void draw(micrasverse::physics::MicrasBody& micrasBody);

    void destroy();
};


} // namespace micrasverse::render

#endif // PLOT_HPP
