#include "physics/box2d_maze.hpp"
#include "constants.hpp"
#include "physics/box2d_rectanglebody.hpp"

#include "box2d/box2d.h"

#include <fstream>
#include <stdexcept>
#include <regex>
#include <iostream>
#include <filesystem>

namespace micrasverse::physics {

// Constructor
Maze::Maze(b2WorldId worldId, const std::string_view filename) : worldId(worldId) {
    this->loadFromFile(filename);
}

// Parse maze from file
void Maze::loadFromFile(const std::string_view filename) {

    std::string filenameStr = std::string(filename);
    
    if (!std::filesystem::exists(filenameStr)) {
        throw std::runtime_error("File does not exist: " + filenameStr);
    }

    std::ifstream file(filenameStr);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open maze file");
    }

    std::string line;
    const float wallOffset = CELL_SIZE / 2.0f;
    int row = 0;

    while (std::getline(file, line)) {

        // Replace sequences of three hyphens with a single hyphen
        line = std::regex_replace(line, std::regex("-{3}"), "-");

        // Replace S with a whitespace
        line = std::regex_replace(line, std::regex("S"), " ");

        // Replace G with a whitespace
        line = std::regex_replace(line, std::regex("G"), " ");
        
        // Replace four consecutive whitespaces by '   :'
        line = std::regex_replace(line, std::regex(" {4}"), "   :");

        // Replace all sequence of exactly 3 whitespaces by a single whitespace
        line = std::regex_replace(line, std::regex(" {3}"), " ");


        size_t col = 0;
        while (col < line.length()) {
            float yPosition = (33 - row -1) * wallOffset + WALL_THICKNESS / 2.0f; // Reverse the y-coordinate by subtracting from the highest value (based on row count)
            float xPosition = col * wallOffset + WALL_THICKNESS / 2.0f;

            switch(line[col]) {
            case 'o':
                // Lattice point
                elements.push_back({'o', {xPosition, yPosition }, {WALL_THICKNESS, WALL_THICKNESS}});
                col++;
                break;
            case '-':
                // Horizontal wall
                elements.push_back({'-', {xPosition, yPosition }, {WALL_SIZE, WALL_THICKNESS}});
                col++; // Move to the next column for the next character
                break;
            case '|':
                // Vertical wall
                elements.push_back({'|', {xPosition, yPosition }, {WALL_THICKNESS, WALL_SIZE}});
                col++;
                break;
            default:
                col++;
                break;
            }
        }
        row++;
    }

    this->createBox2dObjects();
}

const std::vector<Maze::Element>& Maze::getElements() const {
    return elements;
};

// Create Box2D objects
void Maze::createBox2dObjects() {  
    for (const auto& element : this->elements){
        auto rectangleBody = std::make_unique<RectangleBody>(
            worldId,
            element.position,
            element.size,
            b2_staticBody,
            100.0f,
            0.0f,
            0.5f
        );     
        this->mazeBodies.push_back(rectangleBody->getBodyId());
        this->mazeBodiesObjects.push_back(std::move(rectangleBody));
    }
}

void Maze::reloadFromFile(const std::string_view filename) {
    std::string filenameStr = std::string(filename);
    this->destroy(); // Destroy existing Box2D objects
    this->loadFromFile(filenameStr); // Load the maze from the new file
}

// Destroy Box2D objects
void Maze::destroy() {
    for (auto& bodyId : mazeBodies) {
        if(b2Body_IsValid(bodyId)) {
            b2DestroyBody(bodyId); // Destroy the Box2D body
        }
    }
    mazeBodies.clear(); // Clear the list of maze bodies
    elements.clear();   // Clear the list of maze elements
}
} // namespace micrasverse::physics
