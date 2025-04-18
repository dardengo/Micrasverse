#ifndef SIMULATION_ENGINE_HPP
#define SIMULATION_ENGINE_HPP

#include "physics/i_physics_engine.hpp"
#include "constants.hpp"
#include <string>
#include <memory>
#include <vector>

namespace micrasverse::simulation {
    
class SimulationEngine {
public:
    SimulationEngine();

    void updateMazePaths(const std::string& folderPath);

    const std::vector<std::string>& getMazePaths() const;

    const std::string& getCurrentMazePath() const;

    void loadMaze(const std::string& mazeFilePath);

    void togglePause();

    void updateSimulation(float step = micrasverse::STEP);

    void stepThroughSimulation(float step = micrasverse::STEP);

    void resetSimulation();

    void resetSimulation(const std::string& mazeFilePath);

    void setPhysicsEngine(std::shared_ptr<physics::IPhysicsEngine> engine);

    bool isPaused{true};
    bool wasReset{false};
    std::shared_ptr<physics::IPhysicsEngine> physicsEngine;
private:
    std::vector<std::string> mazePaths{};
    std::string currentMazePath;
};

} // namespace micrasverse::simulation


#endif // SIMULATION_ENGINE_HPP
