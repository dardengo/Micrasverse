#include "render/mazerender.hpp"
#include "constants.hpp"
#include "render/material.hpp"
#define GLFW_INCLUDE_NONE   // GLFW include guard
#include "glad/glad.h"      // Use glad instead of GLEW

namespace micrasverse::render {

MazeRender::MazeRender(const std::vector<physics::Maze::Element>& elements): SceneObj(), elements(elements) {}

MazeRender::~MazeRender() {
    for (auto& wall : this->mazeWalls) {
        wall.cleanUp();
    }
    for (auto& wall : this->firmwareWalls) {
        wall.cleanUp();
    }
    for (auto& marker : this->routeMarkers) {
        marker.cleanUp();
    }
}

void MazeRender::init() {
    // Clean up any existing resources
    for (auto& wall : this->mazeWalls) {
        wall.cleanUp();
    }
    this->mazeWalls.clear();
    
    // Initialize standard shader
    this->shader = Shader();
    this->shader.generate("src/render/assets/vertex-core.glsl", "src/render/assets/fragment-core.glsl");
    // Enable blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
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
            glm::vec3(element.position.x, element.position.y, 0.0001f),  // Position with slight z-offset
            glm::vec3(element.size.x, element.size.y, 0.0001f)           // Size
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

void MazeRender::updateFirmwareWalls(const std::shared_ptr<micras::ProxyBridge>& proxyBridge) {
    // Clean up existing firmware walls
    for (auto& wall : this->firmwareWalls) {
        wall.cleanUp();
    }
    this->firmwareWalls.clear();

    
    if (!proxyBridge) return;
    
    // We'll iterate through the maze width/height and create walls where detected by the robot
    for (uint8_t y = 0; y < micras::maze_height; y++) {
        for (uint8_t x = 0; x < micras::maze_width; x++) {
            // For each cell, check if walls are detected in each direction
            // We need to access the internal maze through the ProxyBridge
            
            const micras::nav::GridPoint point{x, y};
            
            // Check walls in all four directions
            for (uint8_t dir = 0; dir < 4; dir++) {
                micras::nav::GridPose pose{point, static_cast<micras::nav::Side>(dir)};
                micras::nav::Costmap<16, 16, 2>::WallState wallState = proxyBridge->get_wall_state(pose);
                
                if (wallState != micras::nav::Costmap<16, 16, 2>::WallState::NO_WALL) {
                    // Calculate wall position based on cell position and direction
                    float posX = 0.0f, posY = 0.0f;
                    float sizeX = 0.0f, sizeY = 0.0f;
                    
                    // Position calculation depends on wall direction
                    switch (dir) {
                        case micras::nav::Side::RIGHT: // East (0)
                            posX = (x + 1) * CELL_SIZE + (WALL_THICKNESS / 2.0f);
                            posY =  y * CELL_SIZE + (CELL_SIZE + WALL_THICKNESS) / 2.0f;
                            sizeX = WALL_THICKNESS;
                            sizeY = WALL_SIZE;
                            break;
                            
                        case micras::nav::Side::UP: // North (1)
                            posX = x * CELL_SIZE + (CELL_SIZE + WALL_THICKNESS) / 2.0f;
                            posY = (y + 1) * CELL_SIZE + (WALL_THICKNESS / 2.0f);
                            sizeX = WALL_SIZE;
                            sizeY = WALL_THICKNESS;
                            break;
                            
                        case micras::nav::Side::LEFT: // West (2)
                            posX = x * CELL_SIZE + WALL_THICKNESS / 2.0f;
                            posY = y * CELL_SIZE + (CELL_SIZE + WALL_THICKNESS) / 2.0f;
                            sizeX = WALL_THICKNESS;
                            sizeY = WALL_SIZE;
                            break;
                            
                        case micras::nav::Side::DOWN: // South (3)
                            posX = x * CELL_SIZE + (CELL_SIZE + WALL_THICKNESS) / 2.0f;
                            posY = y * CELL_SIZE + (WALL_THICKNESS / 2.0f);
                            sizeX = WALL_SIZE;
                            sizeY = WALL_THICKNESS;
                            break;
                    }

                    auto wallState = proxyBridge->get_cell(point).walls[dir];

                    Material material;
                    switch (wallState) {
                        case micras::nav::Costmap<16, 16, 2>::WallState::WALL:
                            material = Material::cyan_plastic;  // Solid walls
                            break;
                        case micras::nav::Costmap<16, 16, 2>::WallState::VIRTUAL:
                            material = Material::yellow_plastic;  // Transparent walls
                            break;
                        case micras::nav::Costmap<16, 16, 2>::WallState::UNKNOWN:
                            material = Material::white_plastic;  // Dotted walls for unknown
                            break;
                    }
                    
                    // Create wall rectangle with the appropriate material
                    Rectangle wall(
                        material,
                        glm::vec3(posX, posY, 0.0002f),                      // Position slightly above physical walls
                        glm::vec3(sizeX, sizeY, 0.0002f)                     // Size (thinner than physical walls)
                    );
                    
                    // Initialize wall
                    wall.init();
                    
                    // Add to collection
                    this->firmwareWalls.push_back(wall);
                }
            }
        }
    }
}

void MazeRender::updateRouteMarkers(const std::shared_ptr<micras::ProxyBridge>& proxyBridge) {
    // Clean up existing route markers
    for (auto& marker : this->routeMarkers) {
        marker.cleanUp();
    }
    this->routeMarkers.clear();
    
    if (!proxyBridge) return;
    
    // Get the best route from the maze
    const auto& best_route = proxyBridge->get_best_route();
    
    // Create a triangle in the center of each cell in the best route
    for (const auto& grid_pose : best_route) {
        // Calculate the center of the cell
        float posX = grid_pose.position.x * CELL_SIZE + (CELL_SIZE / 2.0f);
        float posY = grid_pose.position.y * CELL_SIZE + (CELL_SIZE / 2.0f);
        
        // Triangle size (about 1/3 of the cell size)
        float triangleSize = CELL_SIZE * 0.15f;
        
        // Create a triangle shape
        Rectangle triangle(
            Material::green_plastic,  // Use emerald material for visibility
            glm::vec3(posX, posY, 0.0005f),  // Position slightly above the floor
            glm::vec3(triangleSize, triangleSize, 0.0003f)  // Size
        );
        
        // Set flag to create triangle before initialization
        triangle.isTriangle = true;
        
        // Initialize the triangle
        triangle.init();
        
        // Set rotation based on orientation
        // Side enum: RIGHT(0), UP(1), LEFT(2), DOWN(3)
        float rotationAngle = 0.0f;
        switch (grid_pose.orientation) {
            case micras::nav::Side::RIGHT: // East (0)
                rotationAngle = 270.0f;  // Point right
                break;
            case micras::nav::Side::UP: // North (1)
                rotationAngle = 0.0f; // Point up
                break;
            case micras::nav::Side::LEFT: // West (2)
                rotationAngle = 90.0f; // Point left
                break;
            case micras::nav::Side::DOWN: // South (3)
                rotationAngle = 180.0f; // Point down
                break;
        }
        
        // Convert to radians and set rotation
        triangle.rotation = rotationAngle * (M_PI / 180.0f);
        
        // Add to collection
        this->routeMarkers.push_back(triangle);
    }
}

void MazeRender::render(const glm::mat4 view, const glm::mat4 projection, glm::vec3 position, glm::vec3 cameraPosition, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) {
    // Activate shader with updated view and projection
    this->shader.activate(view, projection);

    // Set lighting parameters for standard shader
    shader.set3Float("lightPosition", position);
    shader.set3Float("viewPosition", cameraPosition);
    shader.set3Float("light.ambient", ambient);
    shader.set3Float("light.diffuse", diffuse);
    shader.set3Float("light.specular", specular);
    
    // 1. Render opaque floor first without blending
    glDisable(GL_BLEND);
    this->renderModel.render(shader, true);
    
    // 2. Render opaque maze walls (alpha = 1.0)
    for (auto& wall : this->mazeWalls) {
        if (wall.material.alpha >= 0.99f) {
            wall.render(shader, true);
        }
    }
    
    // 3. Render best route markers (before transparent objects)
    if (showRouteMarkers) {
        for (auto& marker : this->routeMarkers) {
            marker.render(shader, true);
        }
    }
    
    // 4. Now enable blending for transparent objects
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // 5. Render semi-transparent maze walls after opaque ones
    for (auto& wall : this->mazeWalls) {
        if (wall.material.alpha < 0.99f) {
            wall.render(shader, true);
        }
    }
    
    // 6. Render firmware walls last if needed (they are usually transparent)
    if (showFirmwareWalls) {
        for (auto& wall : this->firmwareWalls) {
            wall.render(shader, true);
        }
    }
    
    // Disable blending when done to avoid affecting other rendering
    glDisable(GL_BLEND);
}

} // namespace micrasverse::render
