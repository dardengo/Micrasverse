#ifndef SIMULATION_ENGINE_CPP
#define SIMULATION_ENGINE_CPP

#include "simulation/simulation_engine.hpp"
#include "physics/box2d_physics_engine.hpp"
#include "io/keyboard.hpp"
#include <filesystem>
#include <iostream>

namespace micrasverse::simulation {

SimulationEngine::SimulationEngine() {
    // Create a Box2DPhysicsEngine which now inherits from IPhysicsEngine
    this->setPhysicsEngine(std::make_shared<micrasverse::physics::Box2DPhysicsEngine>("external/mazefiles/classic/br2024-robochallenge-day3.txt"));
    this->updateMazePaths("external/mazefiles/classic");
    this->currentMazePath = "external/mazefiles/classic/br2024-robochallenge-day3.txt";
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
    
    // If pausing, we don't need to stop the robot controller anymore
    // as it's been removed from the physics engine
}

void SimulationEngine::updateSimulation(float step){
    if (micrasverse::io::Keyboard::keyWentDown(GLFW_KEY_R)) {
        this->togglePause();
    }

    if (!this->isPaused) {
        this->physicsEngine->update(step);
    }
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
