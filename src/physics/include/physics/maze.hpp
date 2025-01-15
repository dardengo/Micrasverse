#ifndef MAZE_HPP
#define MAZE_HPP

#include "models/rectangle.hpp"
#include "render/shader.hpp"

#include "box2d/box2d.h"
#include "glm/glm.hpp"

#include <vector>
#include <string>

namespace micrasverse::physics {

class Maze {
private:
    b2WorldId worldId; // Box2d world ID
    render::Rectangle mazeFloorRender; // Real maze model
    std::vector<render::Rectangle> mazeWalls; // List of maze walls
    micrasverse::render::Shader shader; // Shader program

public:

    struct Element {
        char type;       // '|' for vertical walls, '-' for horizontal walls, 'o' for lattice points
        b2Vec2 position; // Position of the element
        b2Vec2 size;     // Size of the element
    };

    std::vector<Element> elements;  // List of maze elements
    std::vector<b2BodyId> mazeBodies;  // List of maze bodies
    
    // Constructor
    Maze(const b2WorldId worldId, const std::string& filename);

    ~Maze();

    // Parse maze from file
    void loadFromFile(const std::string& filename);

    std::vector<Element> getElements();

    const b2WorldId getWorldId();

    // Create Box2D objects
    void createBox2dObjects();

    void render(const glm::mat4 view, const glm::mat4 projection);

    void cleanUp();

};

} // namespace micrasverse::physics

#endif // MAZE_HPP
