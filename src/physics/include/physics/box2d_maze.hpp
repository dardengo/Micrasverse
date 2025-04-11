#ifndef MAZE_HPP
#define MAZE_HPP

#include "physics/box2d_rectanglebody.hpp"
#include "box2d/box2d.h"

#include <vector>
#include <string>
#include <memory>

namespace micrasverse::physics {

class Maze {
private:
    b2WorldId worldId; // Box2d world ID

public:

    struct Element {
        char type;       // '|' for vertical walls, '-' for horizontal walls, 'o' for lattice points
        b2Vec2 position; // Position of the element
        b2Vec2 size;     // Size of the element
    };

    std::vector<Element> elements;  // List of maze elements
    std::vector<b2BodyId> mazeBodies;  // List of maze bodies
    std::vector<std::unique_ptr<RectangleBody>> mazeBodiesObjects; // List of maze bodies objects
    
    // Constructor
    Maze(const b2WorldId worldId, const std::string& filename);

    // Parse maze from file
    void loadFromFile(const std::string& filename);

    const std::vector<Element>& getElements() const;

    const b2WorldId getWorldId();

    // Create Box2D objects
    void createBox2dObjects();

    // Destroy Box2D objects
    void destroy();

    void reloadFromFile(const std::string& filename);

};

} // namespace micrasverse::physics

#endif // MAZE_HPP
