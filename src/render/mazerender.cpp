#include "render/mazerender.hpp"
#include "config/constants.hpp"

namespace micrasverse::render {

MazeRender::MazeRender(const std::vector<physics::Maze::Element>& elements): SceneObj() {
    
    this->shader = Shader("./render/assets/vertex-core.glsl", "./render/assets/fragment-core.glsl");
    
    this->renderModel = Rectangle(
        glm::vec3(MAZE_FLOOR_HALFWIDTH, MAZE_FLOOR_HALFHEIGHT, 0.01f), // Position of the center
        glm::vec3(MAZE_FLOOR_WIDTH, MAZE_FLOOR_HEIGHT, 0.01f),        // Size
        glm::vec3(15.0f, 15.0f, 15.0f) / 255.0f                       // Color (RGB from 0 to 1)
    );
     
    for (auto& element : elements) {
        micrasverse::render::Rectangle newWall(
            glm::vec3(element.position.x, element.position.y, 0.0f),    // Position of the center
            glm::vec3(element.size.x, element.size.y, 0.01f),           // Size
            glm::vec3(125.0f, 15.0f, 15.0f) / 255.0f                    // Color (RGB from 0 to 1)
        );
        this->mazeWalls.push_back(newWall);
    }
    
    // Initialize renderable objects
    this->renderModel.init();
    for (auto& wall : this->mazeWalls) {
        wall.init();
    }
    
}

MazeRender::~MazeRender() {
    for (auto& wall : this->mazeWalls) {
        wall.cleanUp();
    }
}

void MazeRender::render(const glm::mat4 view, const glm::mat4 projection) {
    this->shader.activate(view, projection);
    
    this->renderModel.render(shader, true);
    
    for (auto& wall : this->mazeWalls) {
        wall.render(shader, true);
    }
}

} // namespace micrasverse::render
