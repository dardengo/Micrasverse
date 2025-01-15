#include "physics/world.hpp"
#include "physics/maze.hpp"
#include "physics/micrasbody.hpp"
#include "render/screen.hpp"
#include "render/keyboard.hpp"

#include "box2d/box2d.h"
#define GLFW_INCLUDE_NONE   // GLFW include guard
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <iostream>

int main() {
    static float deltaTime = 0.0f;
    static float lastFrame = 0.0f;

    
    /*****************
     *** RENDERING ***
     *****************/
    
    // Initialize GLFW and glad, and create window
    micrasverse::render::Screen screen;

    // Check if all went well with the window creation
    if (!screen.init()) {
        glfwTerminate();
        std::cerr << "Failed to create window" << '\n';
        return -1;
    }

    // Set OpenGL viewport size and callback functions + create camera object
    screen.setParameters();


    /***************
     *** PHYSICS ***
     ***************/

    // Create box2d world
    micrasverse::physics::World world(micrasverse::GRAVITY);

    // Create maze
    micrasverse::physics::Maze maze(world.getWorldId(), "../external/mazefiles/classic/br2024-robochallenge-day3.txt");

    // Create Micras
    micrasverse::physics::MicrasBody micrasBody(world.getWorldId());


    /*****************
     *** MAIN LOOP ***
     *****************/
    bool isRunning = false;

    while (!screen.shouldClose()) {
        
        // Calculate delta time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // Process keyboard and mouse inputs for camera
        screen.processInput(deltaTime);

        // Run simulation after pressing 'R'
        if (micrasverse::render::Keyboard::keyWentDown(GLFW_KEY_R)) {
            isRunning = !isRunning;
        }

        // Run simulation
        if (isRunning) {
            world.runStep(1.0f/60.0f, 4);
            micrasBody.update(deltaTime);
        }

        // Clear the screen color buffer
        screen.update(micrasBody);
        
        // Render maze, maze walls and Micras (order defines the z-index if position in z-axis is the same)
        maze.render(screen.view, screen.projection);
        micrasBody.render(screen.view, screen.projection);
        
        // Render GUI
        screen.renderGUI(micrasBody);

        //Swap front (screen) buffer and back (rendering) buffer + poll events like window resize, key and mouse input etc.
        screen.newFrame();        
    }

    return 0;

} //end of main
