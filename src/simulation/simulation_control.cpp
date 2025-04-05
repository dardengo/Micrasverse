#ifndef SIMULATION_CONTROL_CPP
#define SIMULATION_CONTROL_CPP

#include "simulation/simulation_control.hpp"
#include "io/keyboard.hpp"
#include <filesystem>
#include <iostream>

namespace micrasverse::simulation {

SimulationControl::SimulationControl() {
    this->updateMazePaths("external/mazefiles/classic");

    if (!mazePaths.empty()) {
        this->loadMaze("external/mazefiles/classic/br2024-robochallenge-day3.txt");
    } else {
        std::cerr << "[SimulationControl] No maze files found in: external/mazefiles/classic\n";
    }
}

void SimulationControl::updateMazePaths(const std::string& folderPath){
    mazePaths.clear();

    for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            mazePaths.push_back(entry.path().string());
        }
    }

    //std::sort(mazePaths.begin(), mazePaths.end()); // optional: sort alphabetically
}

const std::vector<std::string>& SimulationControl::getMazePaths() const{
    return this->mazePaths;
}

const std::string& SimulationControl::getCurrentMazePath() const{
    return this->currentMazePath;
}

void SimulationControl::loadMaze(const std::string& mazeFilePath){
    this->currentMazePath = mazeFilePath;
    this->physicsEngine = std::make_shared<physics::PhysicsEngine>(mazeFilePath);
}

void SimulationControl::toggleSimulation(){
    this->isRunning = !isRunning;
}

void SimulationControl::updateSimulation(float step){
    if (micrasverse::io::Keyboard::keyWentDown(GLFW_KEY_R)) {
        this->toggleSimulation();
    }

    if (this->isRunning) {
        this->physicsEngine->update(step);
    }
}

void SimulationControl::stepThroughSimulation(float step){
    this->isRunning = false;
    this->physicsEngine->update(step);
}


void SimulationControl::resetSimulation(){
    this->isRunning = false;
    this->physicsEngine->resetMicrasPosition();
}

void SimulationControl::resetSimulation(const std::string& mazeFilePath){
    this->physicsEngine->resetMicrasPosition();
    this->physicsEngine->loadMaze(mazeFilePath);
    this->currentMazePath = mazeFilePath;
    this->isRunning = false;
    this->wasReset = true;
}

} // namespace micrasverse::simulation


#endif // SIMULATION_CONTROL_CPP
