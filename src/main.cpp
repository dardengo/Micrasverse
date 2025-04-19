#include "constants.hpp"
#include "simulation/simulation_engine.hpp"
#include "render/render_engine.hpp"
#include "micras/micras.hpp"
#include "target.hpp"
#include "box2d/box2d.h"
#include <iostream>

int main() {
    
    auto simulationEngine = std::make_shared<micrasverse::simulation::SimulationEngine>();

    b2BodyId bodyId = simulationEngine->physicsEngine->getMicras().getBodyId();

    micras::initializeProxyConfigs(bodyId, b2Body_GetWorld(bodyId));

    micras::Micras micrasController;

    micrasverse::render::RenderEngine renderEngine(simulationEngine);

    while (not renderEngine.screen->shouldClose()) {
        micrasController.update();
        simulationEngine->updateSimulation();
        renderEngine.update();
        renderEngine.renderFrame();
    }

    return 0;

} //end of main
