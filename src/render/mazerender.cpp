#include "render/mazerender.hpp"
#include "config/constants.hpp"
#include "render/material.hpp"

namespace micrasverse::render {

MazeRender::MazeRender(const std::vector<physics::Maze::Element>& elements): SceneObj(), elements(elements) {}

MazeRender::~MazeRender() {
    for (auto& wall : this->mazeWalls) {
        wall.cleanUp();
    }
}

void MazeRender::init() {
    
    this->shader = Shader();
    this->shader.generate("src/render/assets/vertex-core.glsl", "src/render/assets/fragment-core.glsl");
    
    this->renderModel = Rectangle(
        Material::black_rubber,                                        // Material
        glm::vec3(MAZE_FLOOR_HALFWIDTH, MAZE_FLOOR_HALFHEIGHT, 0.01f), // Position of the center
        glm::vec3(MAZE_FLOOR_WIDTH, MAZE_FLOOR_HEIGHT, 0.01f)          // Size
    );
     
    for (auto& element : this->elements) {
        Rectangle newWall(
            Material::red_plastic,                                      // Material
            glm::vec3(element.position.x, element.position.y, 0.0f),    // Position of the center
            glm::vec3(element.size.x, element.size.y, 0.01f)            // Size
        );
        this->mazeWalls.push_back(newWall);
    }
    
    // Initialize renderable objects
    this->renderModel.init();
    for (auto& wall : this->mazeWalls) {
        wall.init();
    }


}

void MazeRender::render(const glm::mat4 view, const glm::mat4 projection, glm::vec3 position, glm::vec3 cameraPosition, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) {
    this->shader.activate(view, projection);

    shader.set3Float("lightPosition", position);
    shader.set3Float("viewPosition", cameraPosition);
    shader.set3Float("light.ambient", ambient);
    shader.set3Float("light.diffuse", diffuse);
    shader.set3Float("light.specular", specular);
    
    this->renderModel.render(shader, true);
    
    for (auto& wall : this->mazeWalls) {
        wall.render(shader, true);
    }
}

} // namespace micrasverse::render
