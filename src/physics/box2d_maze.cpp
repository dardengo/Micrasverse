#include "physics/box2d_maze.hpp"
#include "config/constants.hpp"
#include "physics/box2d_rectanglebody.hpp"

#include "box2d/box2d.h"

#include <fstream>
#include <stdexcept>
#include <regex>
#include <iostream>
#include <filesystem>

namespace micrasverse::physics {

// Constructor
Maze::Maze(b2WorldId worldId, const std::string& filename) {
    std::cout << "========== MAZE CREATION START ==========" << std::endl;
    std::cout << "Maze constructor received world ID with index: " << worldId.index1 << std::endl;
    
    // Validate the world ID immediately
    bool isValidWorld = b2World_IsValid(worldId);
    std::cout << "World validity check in Maze constructor: " << (isValidWorld ? "VALID" : "INVALID") << std::endl;
    
    if (!isValidWorld) {
        std::cerr << "ERROR: Invalid world ID in Maze constructor" << std::endl;
        throw std::runtime_error("Invalid world ID in Maze constructor");
    }
    
    // Store the world ID
    this->worldId = worldId;
    std::cout << "Stored world ID with index: " << this->worldId.index1 << std::endl;
    
    std::cout << "Loading maze from file: " << filename << std::endl;
    this->loadFromFile(filename);
    
    std::cout << "Creating Box2D objects for maze..." << std::endl;
    this->createBox2dObjects();
    
    std::cout << "Maze created successfully" << std::endl;
    std::cout << "========== MAZE CREATION END ==========" << std::endl;
}

// Parse maze from file
void Maze::loadFromFile(const std::string& filename) {
    
    if (!std::filesystem::exists(filename)) {
        throw std::runtime_error("File does not exist: " + filename);
    }

    std::ifstream file(filename);
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
    std::cout << "========== MAZE OBJECTS CREATION START ==========" << std::endl;
    std::cout << "Using world ID with index: " << worldId.index1 << std::endl;
    
    bool isValidWorld = b2World_IsValid(worldId);
    std::cout << "World validity check before creating maze objects: " << (isValidWorld ? "VALID" : "INVALID") << std::endl;
    
    if (!isValidWorld) {
        std::cerr << "ERROR: Invalid world ID in createBox2dObjects" << std::endl;
        throw std::runtime_error("Invalid world ID in createBox2dObjects");
    }
    
    std::cout << "Creating " << this->elements.size() << " maze elements" << std::endl;
    
    for (const auto& element : this->elements){
        try {
            std::cout << "Creating maze element at position (" << element.position.x << ", " << element.position.y 
                      << ") of type '" << element.type << "' and size (" << element.size.x << ", " << element.size.y << ")" << std::endl;
                      
            auto rectangleBody = std::make_unique<RectangleBody>(
                worldId,
                element.position,
                element.size,
                b2_staticBody,
                100.0f,
                0.3f,
                0.5f
            );
            
            b2BodyId bodyId = rectangleBody->getBodyId();
            std::cout << "Created maze body with ID index: " << bodyId.index1 << std::endl;
            
            this->mazeBodies.push_back(bodyId);
            this->mazeBodiesObjects.push_back(std::move(rectangleBody));
        } catch (const std::exception& e) {
            std::cerr << "ERROR creating maze element: " << e.what() << std::endl;
        }
    }
    
    std::cout << "Created " << this->mazeBodies.size() << " maze bodies" << std::endl;
    std::cout << "========== MAZE OBJECTS CREATION END ==========" << std::endl;
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

void Maze::reloadFromFile(const std::string& filename) {
    this->destroy(); // Destroy existing Box2D objects
    this->loadFromFile(filename); // Load the maze from the new file
}

} // namespace micrasverse::physics
