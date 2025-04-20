#include "constants.hpp"
#include "simulation/simulation_engine.hpp"
#include "render/render_engine.hpp"
#include "micras/micras.hpp"
#include "micras/proxy/proxy_bridge.hpp"
#include "target.hpp"
#include "box2d/box2d.h"

int main() {
    // Create simulation engine
    auto simulationEngine = std::make_shared<micrasverse::simulation::SimulationEngine>();
    
    // Get the Micras body from physics engine
    auto& micrasBody = simulationEngine->physicsEngine->getMicras();
    
    // Initialize proxy configurations
    micras::initializeProxyConfigs(&micrasBody);

    // Create Micras controller and proxy bridge
    micras::Micras micrasController;
    auto proxyBridge = std::make_shared<micras::ProxyBridge>(micrasController, micrasBody);

    // Initialize render engine
    micrasverse::render::RenderEngine renderEngine(simulationEngine);
    renderEngine.screen->getGUI().setProxyBridge(proxyBridge);

    // Main loop
    while (not renderEngine.screen->shouldClose()) {
        micrasController.update();
        simulationEngine->updateSimulation();
        renderEngine.update();
        renderEngine.renderFrame();
    }

    return 0;
}
