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
    // Clean up any existing resources
    for (auto& wall : this->mazeWalls) {
        wall.cleanUp();
    }
    this->mazeWalls.clear();
    
    // Initialize shader
    this->shader = Shader();
    this->shader.generate("src/render/assets/vertex-core.glsl", "src/render/assets/fragment-core.glsl");
    
    // Create floor - the black ground
    this->renderModel = Rectangle(
        Material::black_rubber,                                        // Material
        glm::vec3(MAZE_FLOOR_HALFWIDTH, MAZE_FLOOR_HALFHEIGHT, 0.0f), // Position of the center
        glm::vec3(MAZE_FLOOR_WIDTH, MAZE_FLOOR_HEIGHT, 0.01f)          // Size
    );
    
    // Initialize the floor
    this->renderModel.init();
    
    // Create and initialize the maze walls one by one
    for (const auto& element : this->elements) {
        Rectangle wall(
            Material::red_plastic,                                     // Material (red walls)
            glm::vec3(element.position.x, element.position.y, 0.02f),  // Position with slight z-offset
            glm::vec3(element.size.x, element.size.y, 0.01f)           // Size
        );
        
        // Initialize this wall before adding to vector
        wall.init();
        
        // Add to our collection
        this->mazeWalls.push_back(wall);
    }
}

void MazeRender::reloadElements(const std::vector<physics::Maze::Element>& elements) {
    this->elements = elements;
    this->init();
}

void MazeRender::render(const glm::mat4 view, const glm::mat4 projection, glm::vec3 position, glm::vec3 cameraPosition, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) {
    // Activate shader with updated view and projection
    this->shader.activate(view, projection);

    // Set lighting parameters
    shader.set3Float("lightPosition", position);
    shader.set3Float("viewPosition", cameraPosition);
    shader.set3Float("light.ambient", ambient);
    shader.set3Float("light.diffuse", diffuse);
    shader.set3Float("light.specular", specular);
    
    // First render the floor (explicitly controlling depth)
    this->renderModel.render(shader, true);
    
    // Then render all walls
    for (auto& wall : this->mazeWalls) {
        wall.render(shader, true);
    }
}

} // namespace micrasverse::render
