#ifndef MAZE_RENDER_HPP
#define MAZE_RENDER_HPP

#include "physics/box2d_maze.hpp"
#include "render/sceneobj.hpp"
#include "micras/proxy/proxy_bridge.hpp"

#include <vector>

namespace micrasverse::render {

class MazeRender: public SceneObj {

public:
    std::vector<render::Rectangle> mazeWalls; // List of maze walls
    std::vector<render::Rectangle> firmwareWalls; // List of firmware detected walls

    MazeRender(const std::vector<physics::Maze::Element>& elements);

    ~MazeRender();

    void init();

    void reloadElements(const std::vector<physics::Maze::Element>& elements);

    void updateFirmwareWalls(const std::shared_ptr<micras::ProxyBridge>& proxyBridge);

    void render(const glm::mat4 view, const glm::mat4 projection, glm::vec3 position, glm::vec3 cameraPosition, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) override;

    bool showFirmwareWalls = true;

private:
    std::vector<physics::Maze::Element> elements;

};

} // namespace micrasverse::render


#endif // MAZE_RENDER_HPP
