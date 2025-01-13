#include "physics/maze.h"
#include "core/constants.h"
#include "physics/rectanglebody.h"

#include "box2d/box2d.h"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <regex>

namespace micrasverse::physics {

// Constructor
Maze::Maze(const b2WorldId worldId) {
    this->worldId = worldId;
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
}

const b2WorldId Maze::getWorldId() {
    return worldId;
}

std::vector<Maze::Element> Maze::getElements() {
    return elements;
};

// Create Box2D objects
void Maze::createBox2dObjects() {

    //RectangleBody mazeBody(worldId, (b2Vec2){MAZE_FLOOR_HALFWIDTH, MAZE_FLOOR_HALFHEIGHT}, (b2Vec2){MAZE_FLOOR_WIDTH, MAZE_FLOOR_HEIGHT}, b2_staticBody, (b2MassData){0.0f, (b2Vec2){MAZE_FLOOR_HALFWIDTH,MAZE_FLOOR_HALFHEIGHT}, 0.0f}, 0.0f, 1.0f);

    //mazeBodies.push_back(mazeBody.getBodyId());

    for (const auto& element : elements){
        //::cout << "Element at:" << " (" << element.position.x - element.size.x / 2.0f << ", " << element.position.y - element.size.y / 2.0f << ") " << " (" << element.position.x + element.size.x /2.0f << ", " << element.position.y + element.size.y / 2.0f << ") " << '\n';
        RectangleBody elementBody(worldId, (b2Vec2){element.position.x, element.position.y}, element.size, b2_staticBody, (b2MassData){0.0f, (b2Vec2){element.size.x / 2.0f, element.size.y / 2.0f}, 0.0f}, 0.0f, 1.0f);
        mazeBodies.push_back(elementBody.getBodyId());
        //std::cout << "Body at:" << " (" << elementBody.getPosition().x - elementBody.getSize().x / 2.0f << ", " << elementBody.getPosition().y - elementBody.getSize().y / 2.0f << ") " << " (" << elementBody.getPosition().x + elementBody.getSize().x /2.0f << ", " << elementBody.getPosition().y + elementBody.getSize().y /2.0f << ") " << '\n';
    }
    
}

} // namespace micrasverse::physics
