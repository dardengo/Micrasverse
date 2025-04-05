#ifndef GUI_HPP
#define GUI_HPP

#include "simulation/simulation_control.hpp"
#include "GUI/plot.hpp"
#include "physics/micrasbody.hpp"

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

    void setSimulationControl(const std::shared_ptr<micrasverse::simulation::SimulationControl>& simulationControl);

    void init(GLFWwindow* window);
    
    void update();

    void getMazeFiles(std::vector<std::string>& mazeFiles);

    void draw(micrasverse::physics::MicrasBody& micrasBody);

    void render();
    
    void destroy();

private:
    std::shared_ptr<micrasverse::simulation::SimulationControl> simulationControl;
};

} // namespace micrasverse::render

#endif // GUI_HPP
