#ifndef SIMULATION_ENGINE_CPP
#define SIMULATION_ENGINE_CPP

#include "simulation/simulation_engine.hpp"
#include "physics/box2d_physics_engine.hpp"
#include "io/keyboard.hpp"
#include <filesystem>

namespace micrasverse::simulation {

SimulationEngine::SimulationEngine() {
    this->setPhysicsEngine(std::make_shared<micrasverse::physics::Box2DPhysicsEngine>(DEFAULT_MAZE_PATH));
    this->updateMazePaths("external/mazefiles/classic");
    this->currentMazePath = DEFAULT_MAZE_PATH;
}

void SimulationEngine::updateMazePaths(const std::string& folderPath){
    mazePaths.clear();

    for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            mazePaths.push_back(entry.path().string());
        }
    }

    std::sort(mazePaths.begin(), mazePaths.end());
}

const std::vector<std::string>& SimulationEngine::getMazePaths() const{
    return this->mazePaths;
}

const std::string& SimulationEngine::getCurrentMazePath() const{
    return this->currentMazePath;
}

void SimulationEngine::loadMaze(const std::string& mazeFilePath){
    this->currentMazePath = mazeFilePath;
    this->physicsEngine->loadMaze(mazeFilePath);
    this->wasReset = true;
}

void SimulationEngine::togglePause(){
    this->isPaused = !isPaused;
}

void SimulationEngine::updateSimulation(float step){
    this->physicsEngine->update(step);
}

void SimulationEngine::stepThroughSimulation(float step){
    this->isPaused = true;
    this->physicsEngine->update(step);
}


void SimulationEngine::resetSimulation(){
    this->isPaused = true;
    this->physicsEngine->resetMicrasPosition();
}

void SimulationEngine::resetSimulation(const std::string& mazeFilePath){
    this->physicsEngine->resetMicrasPosition();
    this->physicsEngine->loadMaze(mazeFilePath);
    this->currentMazePath = mazeFilePath;
    this->isPaused = true;
    this->wasReset = true;
}

void SimulationEngine::setPhysicsEngine(std::shared_ptr<physics::IPhysicsEngine> engine) {
    this->physicsEngine = std::move(engine);
}

} // namespace micrasverse::simulation


#endif // SIMULATION_ENGINE_CPP
