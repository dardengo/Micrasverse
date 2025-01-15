#include "physics/maze.hpp"
#include "core/constants.hpp"
#include "physics/rectanglebody.hpp"
#include "models/rectangle.hpp"

#include "box2d/box2d.h"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <regex>

namespace micrasverse::physics {

// Constructor
Maze::Maze(const b2WorldId worldId, const std::string& filename) {
    this->worldId = worldId;

    // Create renderable object for the maze floor
    micrasverse::render::Rectangle mazeFloorRender(
        glm::vec3(MAZE_FLOOR_HALFWIDTH, MAZE_FLOOR_HALFHEIGHT, 0.01f), // Position of the center
        glm::vec3(MAZE_FLOOR_WIDTH, MAZE_FLOOR_HEIGHT, 0.01f),        // Size
        glm::vec3(15.0f, 15.0f, 15.0f) / 255.0f                       // Color (RGB from 0 to 1)
    );

    this->mazeFloorRender = mazeFloorRender;

    micrasverse::render::Shader flatColorShader("./render/assets/vertex-core.glsl", "./render/assets/fragment-core.glsl");

    this->shader = flatColorShader;

    this->loadFromFile(filename);
}

Maze::~Maze() {
    this->cleanUp();
}

// Parse maze from file
void Maze::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open maze file");
    }

    std::string line;
    const float wallOffset = CELL_SIZE / 2.0f;
    int row = 0;

    while (std::getline(file, line)) {

        // Replace sequences of three or more hyphens with a single hyphen
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

    // Create renderable objects for walls and lattice points
    this->mazeWalls.reserve(this->elements.size());
    
    for (auto& element : this->elements) {
        micrasverse::render::Rectangle newWall(
            glm::vec3(element.position.x, element.position.y, 0.0f),    // Position of the center
            glm::vec3(element.size.x, element.size.y, 0.01f),           // Size
            glm::vec3(125.0f, 15.0f, 15.0f) / 255.0f                    // Color (RGB from 0 to 1)
        );
        this->mazeWalls.push_back(newWall);
    }

    // Initialize renderable objects
    this->mazeFloorRender.init();
    for (auto& wall : this->mazeWalls) {
        wall.init();
    }
}

const b2WorldId Maze::getWorldId() {
    return worldId;
}

std::vector<Maze::Element> Maze::getElements() {
    return elements;
};

// Create Box2D objects
void Maze::createBox2dObjects() {

    for (const auto& element : this->elements){
        RectangleBody elementBody(worldId, (b2Vec2){element.position.x, element.position.y}, element.size, b2_staticBody, (b2MassData){0.0f, (b2Vec2){element.size.x / 2.0f, element.size.y / 2.0f}, 0.0f}, 0.0f, 1.0f);
        mazeBodies.push_back(elementBody.getBodyId());
    }
    
}

void Maze::render(const glm::mat4 view, const glm::mat4 projection) {

    this->shader.activate(view, projection);
    
    this->mazeFloorRender.render(shader, true);

    for (auto& wall : this->mazeWalls) {
        wall.render(shader, true);
    }
}

void Maze::cleanUp() {
    this->mazeFloorRender.cleanUp();
    
    for (auto& wall : this->mazeWalls) {
        wall.cleanUp();
    }
}

} // namespace micrasverse::physics
