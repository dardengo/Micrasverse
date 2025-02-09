#ifndef MAZE_RENDER_HPP
#define MAZE_RENDER_HPP

#include "physics/maze.hpp"
#include "render/sceneobj.hpp"

#include <vector>

namespace micrasverse::render {

class MazeRender: public SceneObj {

public:
    std::vector<render::Rectangle> mazeWalls; // List of maze walls

    MazeRender(const std::vector<physics::Maze::Element>& elements);

    ~MazeRender();

    void render(const glm::mat4 view, const glm::mat4 projection) override;
};

} // namespace micrasverse::render


#endif // MAZE_RENDER_HPP
