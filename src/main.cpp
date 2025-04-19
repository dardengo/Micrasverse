#include "constants.hpp"
#include "simulation/simulation_engine.hpp"
#include "render/render_engine.hpp"
#include "micras/micras.hpp"
#include "micras/proxy/proxy_bridge.hpp"
#include "target.hpp"
#include "box2d/box2d.h"
#include <iostream>

int main() {
    
    auto simulationEngine = std::make_shared<micrasverse::simulation::SimulationEngine>();

    b2BodyId bodyId = simulationEngine->physicsEngine->getMicras().getBodyId();

    micras::initializeProxyConfigs(bodyId, b2Body_GetWorld(bodyId));

    micras::Micras micrasController;
    auto& micrasBody = simulationEngine->physicsEngine->getMicras();
    auto proxyBridge = std::make_shared<micras::ProxyBridge>(micrasController, micrasBody);

    micrasverse::render::RenderEngine renderEngine(simulationEngine);
    renderEngine.screen->getGUI().setProxyBridge(proxyBridge);

    while (not renderEngine.screen->shouldClose()) {
        micrasController.update();
        simulationEngine->updateSimulation();
        renderEngine.update();
        renderEngine.renderFrame();
    }

    return 0;

} //end of main
