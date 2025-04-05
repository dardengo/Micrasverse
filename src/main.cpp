#include "simulation/simulation_control.hpp"
#include "render/render_engine.hpp"
#include "config/constants.hpp"

int main() {
    auto simulationControl = std::make_shared<micrasverse::simulation::SimulationControl>();

    micrasverse::render::RenderEngine renderEngine(simulationControl);

    while (not renderEngine.screen->shouldClose()) {
        simulationControl->updateSimulation(micrasverse::STEP);
        renderEngine.update();
        renderEngine.renderFrame();
    }

    return 0;

} //end of main
