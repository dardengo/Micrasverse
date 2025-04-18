#include "constants.hpp"
#include "simulation/simulation_engine.hpp"
#include "render/render_engine.hpp"

int main() {
    
    auto simulationEngine = std::make_shared<micrasverse::simulation::SimulationEngine>();

    micrasverse::render::RenderEngine renderEngine(simulationEngine);

    while (not renderEngine.screen->shouldClose()) {
        // Update simulation
        simulationEngine->updateSimulation();
        
        // Update and render
        renderEngine.update();
        renderEngine.renderFrame();
    }

    return 0;

} //end of main
