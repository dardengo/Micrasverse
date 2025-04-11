#ifndef GUI_HPP
#define GUI_HPP

#include "simulation/simulation_engine.hpp"
#include "GUI/plot.hpp"
#include "physics/box2d_micrasbody.hpp"

#define GLFW_INCLUDE_NONE   // GLFW include guard
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <vector>
#include <string>

namespace micrasverse::render {

class GUI {
public:
    Plot plot;
    
    GUI();

    void setSimulationEngine(const std::shared_ptr<micrasverse::simulation::SimulationEngine>& simulationEngine);

    void init(GLFWwindow* window);
    
    void update();

    void getMazeFiles(std::vector<std::string>& mazeFiles);

    void draw(micrasverse::physics::MicrasBody& micrasBody);

    void render();
    
    void destroy();

private:
    std::shared_ptr<micrasverse::simulation::SimulationEngine> simulationEngine;
};

} // namespace micrasverse::render

#endif // GUI_HPP
