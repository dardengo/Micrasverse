#include "physics/world.hpp"
#include "physics/maze.hpp"
#include "physics/micrasbody.hpp"
#include "render/screen.hpp"
#include "io/keyboard.hpp"
#include "render/micrasrender.hpp"
#include "render/argbrender.hpp"
#include "render/lidarrender.hpp"
#include "render/mazerender.hpp"

#include "box2d/box2d.h"
#define GLFW_INCLUDE_NONE   // GLFW include guard
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <iostream>
#include <vector>

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


    /*******************
     *** SCENE SETUP ***
     *******************/

    // Create Maze scene object
    micrasverse::render::MazeRender mazeRender(maze.getElements());

    // Create Micras scene object
    micrasverse::render::MicrasRender micrasRender(micrasBody.getPosition(), micrasBody.getSize());

    // Create ARGB scene objects
    std::vector<micrasverse::render::ARGBRender> argbsSceneObjs;

    for (auto& argb : micrasBody.argb.argbs) {
        argbsSceneObjs.push_back(micrasverse::render::ARGBRender(argb.worldPosition, argb.size, argb.lightColorArray, argb.baseColorArray, argb.isOn));
    }  

    // Create Lidar scene objects
    std::vector<micrasverse::render::LidarRender> lidarsSceneObjs;

    for (auto& lidar : micrasBody.wallSensors.get_sensors()) {
        lidarsSceneObjs.push_back(micrasverse::render::LidarRender(lidar.rayMidPoint, lidar.rayDirection, lidar.reading));
    }


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
        if (micrasverse::io::Keyboard::keyWentDown(GLFW_KEY_R)) {
            isRunning = !isRunning;
        }

        // Run simulation
        if (isRunning) {
            world.runStep(1.0f/60.0f, 4);
            micrasBody.update(1.0f/60.0f);

            // Update scene objects
            micrasRender.update(micrasBody.getPosition(), micrasBody.getRotation());
            
            for (size_t i = 0; i < micrasBody.argb.argbs.size(); i++) {
                argbsSceneObjs[i].update(micrasBody.argb.argbs[i].worldPosition, micrasBody.getRotation(), micrasBody.argb.argbs[i].lightColorArray, micrasBody.argb.argbs[i].isOn);
            }

            for (size_t i = 0; i < micrasBody.wallSensors.get_sensors().size(); i++) {
                lidarsSceneObjs[i].update(micrasBody.wallSensors.get_sensors()[i].rayMidPoint, micrasBody.wallSensors.get_sensors()[i].rayDirection, micrasBody.wallSensors.get_sensors()[i].reading);
            }

        }

        // Clear the screen color buffer
        screen.update(micrasBody);
        
        // Render maze, maze walls and Micras (order defines the z-index if position in z-axis is the same)
        mazeRender.render(screen.view, screen.projection);
        micrasRender.render(screen.view, screen.projection);

        for (auto& argbSceneObj: argbsSceneObjs) {
            argbSceneObj.render(screen.view, screen.projection);
        }

        for (auto& lidarSceneObj: lidarsSceneObjs) {
            lidarSceneObj.render(screen.view, screen.projection);
        }
        
        // Render GUI
        screen.renderGUI(micrasBody);

        //Swap front (screen) buffer and back (rendering) buffer + poll events like window resize, key and mouse input etc.
        screen.newFrame();        
    }

    return 0;

} //end of main
