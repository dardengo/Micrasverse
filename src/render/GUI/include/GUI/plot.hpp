#ifndef PLOT_HPP
#define PLOT_HPP

#include "physics/box2d_micrasbody.hpp"
#include "micras/proxy/proxy_bridge.hpp"

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

    void draw(micrasverse::physics::Box2DMicrasBody& micrasBody, micras::ProxyBridge& proxyBridge, bool simulationIsPaused = false);

    void destroy();
    
    bool showPlots;
    float updateInterval;
    float timeSinceLastUpdate;
};


} // namespace micrasverse::render

#endif // PLOT_HPP
