#ifndef RENDER_ENGINE_HPP
#define RENDER_ENGINE_HPP

#include "simulation/simulation_engine.hpp"
#include "physics/i_physics_engine.hpp"
#include "render/screen.hpp"
#include "render/micrasrender.hpp"
#include "render/argbrender.hpp"
#include "render/lidarrender.hpp"
#include "render/mazerender.hpp"
#include "models/led.hpp"
#include "render/shader.hpp"
#include "render/boundingbox.hpp"
#include "micras/proxy/proxy_bridge.hpp"

#include <vector>
#include <memory>
#include <array>


namespace micrasverse::render {

class RenderEngine {
    public:
    RenderEngine(const std::shared_ptr<micrasverse::simulation::SimulationEngine>& simulationEngine);

    void setProxyBridge(const std::shared_ptr<micras::ProxyBridge>& proxyBridge);

    void update();

    void renderFrame();

    void reloadMaze(const std::string& mazeFilePath);

    std::unique_ptr<Screen> screen;
    
    private:
    std::shared_ptr<micras::ProxyBridge> proxyBridge;

    // Frustum culling check for optimization
    bool isObjectInView(const BoundingBox& bbox);
    
    // Performance optimization flags
    bool isFullscreen;
    bool frustumCullingEnabled;
    
    std::shared_ptr<micrasverse::simulation::SimulationEngine> simulationEngine;
    std::unique_ptr<MazeRender> mazeRender;
    std::unique_ptr<MicrasRender> micrasRender;
    std::vector<std::unique_ptr<ARGBRender>> argbsSceneObjs;
    std::vector<std::unique_ptr<LidarRender>> lidarsSceneObjs;
    std::unique_ptr<Shader> lightShader;

    friend class GUI;
};

} // namespace micrasverse::render
#endif // RENDER_ENGINE_HPP
