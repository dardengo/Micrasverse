#include "constants.hpp"
#include "simulation/simulation_engine.hpp"
#include "render/render_engine.hpp"
#include "micras/micras.hpp"
#include "micras/proxy/proxy_bridge.hpp"
#include "target.hpp"
#include "box2d/box2d.h"
#include "io/keyboard.hpp"

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
    auto renderEngine = std::make_shared<micrasverse::render::RenderEngine>(simulationEngine);
    renderEngine->setProxyBridge(proxyBridge);
    renderEngine->screen->setProxyBridge(proxyBridge);
    renderEngine->screen->setRenderEngine(renderEngine.get());

    // Simple counter for controlling render frequency
    const int physicsStepsPerFrame = 20;
    int stepCounter = 0;
    
    // Initial render
    renderEngine->update();
    renderEngine->renderFrame();

    // Main loop
    while (not renderEngine->screen->shouldClose()) {
        
        if (micrasverse::io::Keyboard::keyWentDown(GLFW_KEY_R)) {
            simulationEngine->togglePause();
        }

        if (!simulationEngine->isPaused) {
            micrasController.update();
            simulationEngine->updateSimulation();
            stepCounter++;
            
            if (stepCounter >= physicsStepsPerFrame) {
                renderEngine->update();
                renderEngine->renderFrame();
                stepCounter = 0;
            }
        } else {
            renderEngine->update();
            renderEngine->renderFrame();
        }
    }

    return 0;
}
