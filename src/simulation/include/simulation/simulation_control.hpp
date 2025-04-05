#ifndef SIMULATION_CONTROL_HPP
#define SIMULATION_CONTROL_HPP

#include "physics/physics_engine.hpp"
#include "config/constants.hpp"
#include <string>
#include <memory>
#include <vector>

namespace micrasverse::simulation {

class SimulationControl {
public:
    // Constructor
    SimulationControl();

    // Update Maze Paths
    void updateMazePaths(const std::string& folderPath);

    // Get Maze Paths
    const std::vector<std::string>& getMazePaths() const;

    // Get Current Maze Path
    const std::string& getCurrentMazePath() const;

    // Load Maze
    void loadMaze(const std::string& mazeFilePath);

    // Method to pause the simulation
    void toggleSimulation();

    // Method to update the simulation
    void updateSimulation(float step = micrasverse::STEP);


    void stepThroughSimulation(float step = micrasverse::STEP);

    // Method to reset the simulation
    void resetSimulation();

    // Method to reset the simulation with a specific maze file path
    void resetSimulation(const std::string& mazeFilePath);

    std::shared_ptr<physics::PhysicsEngine> physicsEngine;
    
    bool isRunning{false};
    bool wasReset{false};

    private:
    // Private member variables to manage simulation state
    std::vector<std::string> mazePaths{};
    std::string currentMazePath;
};

} // namespace micrasverse::simulation


#endif // SIMULATION_CONTROL_HPP
