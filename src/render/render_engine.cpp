#ifndef RENDER_ENGINE_CPP
#define RENDER_ENGINE_CPP

#include "render/render_engine.hpp"
#include <iostream>

namespace micrasverse::render {

    RenderEngine::RenderEngine(const std::shared_ptr<micrasverse::simulation::SimulationControl>& simulationControl) : simulationControl(simulationControl) {
        this->screen = std::make_unique<Screen>(this->simulationControl);

        // Check if all went well with the window creation
        if (!this->screen->init()) {
            glfwTerminate();
            std::cerr << "Failed to create window" << '\n';
            this->screen->setShouldClose(true);
        }

        
        // Set OpenGL viewport size and callback functions
        screen->setParameters();

        this->lightShader = std::make_unique<Shader>();
        this->lightShader->generate("src/render/assets/vertex-core.glsl", "src/render/assets/fragment-led.glsl");
        this->mazeRender = std::make_unique<MazeRender>(this->simulationControl->physicsEngine->p_Maze.getElements());
        this->mazeRender->init();
        this->micrasRender = std::make_unique<MicrasRender>(this->simulationControl->physicsEngine->p_Micras.getPosition(), simulationControl->physicsEngine->p_Micras.getSize());
        
        this->light = std::make_unique<Led>(
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(micrasverse::MAZE_FLOOR_HALFWIDTH, micrasverse::MAZE_FLOOR_HALFHEIGHT, screen->camera.position.z - 0.01f),
            glm::vec3(micrasverse::MAZE_FLOOR_HALFWIDTH, micrasverse::MAZE_FLOOR_HALFHEIGHT, 0.0f)
        );

        // Create ARGB scene objects
        for (auto& argb : this->simulationControl->physicsEngine->p_Micras.argb.argbs) {
            this->argbsSceneObjs.emplace_back(std::make_unique<ARGBRender>(argb.worldPosition, argb.size, argb.lightColorArray, argb.baseColorArray, argb.isOn, screen->camera));
        }

        // Create Lidar scene objects
        for (auto& lidar : this->simulationControl->physicsEngine->p_Micras.wallSensors.get_sensors()) {
            this->lidarsSceneObjs.emplace_back(std::make_unique<LidarRender>(lidar.rayMidPoint, lidar.rayDirection, lidar.reading, screen->camera));
        }
    }

    void RenderEngine::update() {
        screen->processInput();
        
        micrasRender->update(this->simulationControl->physicsEngine->p_Micras.getPosition(), this->simulationControl->physicsEngine->p_Micras.getRotation());
        
        
        for (size_t i = 0; i < this->simulationControl->physicsEngine->p_Micras.argb.argbs.size(); i++) {
            this->argbsSceneObjs[i]->update(this->simulationControl->physicsEngine->p_Micras.argb.argbs[i].worldPosition, this->simulationControl->physicsEngine->p_Micras.getRotation(), this->simulationControl->physicsEngine->p_Micras.argb.argbs[i].lightColorArray, this->simulationControl->physicsEngine->p_Micras.argb.argbs[i].isOn);
        }

        for (size_t i = 0; i < this->simulationControl->physicsEngine->p_Micras.wallSensors.get_sensors().size(); i++) {
            this->lidarsSceneObjs[i]->update(this->simulationControl->physicsEngine->p_Micras.wallSensors.get_sensors()[i].rayMidPoint, this->simulationControl->physicsEngine->p_Micras.wallSensors.get_sensors()[i].rayDirection, this->simulationControl->physicsEngine->p_Micras.wallSensors.get_sensors()[i].reading);
        }
    }

    void RenderEngine::renderFrame() {
        // Clear the screen color buffer
        screen->update(this->simulationControl->physicsEngine->p_Micras);
        
        // Render maze, maze walls and Micras (order defines the z-index if position in z-axis is the same)
        mazeRender->render(screen->view, screen->projection, light->position, screen->camera.position, light->ambient, light->diffuse, light->specular);
        micrasRender->render(screen->view, screen->projection, light->position, screen->camera.position, light->ambient, light->diffuse, light->specular);

        for (auto& argbSceneObj: argbsSceneObjs) {
            argbSceneObj->render(screen->view, screen->projection);
        }

        for (auto& lidarSceneObj: lidarsSceneObjs) {
            lidarSceneObj->render(screen->view, screen->projection);
        }

        // Render light
        lightShader->activate(screen->view, screen->projection);
        light->render(*lightShader, true);

        // Render GUI
        screen->renderGUI(this->simulationControl->physicsEngine->p_Micras);

        //Swap front (screen) buffer and back (rendering) buffer + poll events like window resize, key and mouse input etc.
        screen->newFrame();      
    }


} // namespace micrasverse::render

#endif // RENDER_ENGINE_CPP
