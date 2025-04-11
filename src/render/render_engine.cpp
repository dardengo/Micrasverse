#ifndef RENDER_ENGINE_CPP
#define RENDER_ENGINE_CPP

#include "render/render_engine.hpp"
#include <iostream>

namespace micrasverse::render {

    RenderEngine::RenderEngine(const std::shared_ptr<micrasverse::simulation::SimulationEngine>& simulationEngine) : simulationEngine(simulationEngine) {
        this->screen = std::make_unique<Screen>(this->simulationEngine);

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
        this->mazeRender = std::make_unique<MazeRender>(this->simulationEngine->physicsEngine->getMaze().getElements());
        this->mazeRender->init();
        this->micrasRender = std::make_unique<MicrasRender>(this->simulationEngine->physicsEngine->getMicras().getPosition(), simulationEngine->physicsEngine->getMicras().getSize());
        
        this->light = std::make_unique<Led>(
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(micrasverse::MAZE_FLOOR_HALFWIDTH, micrasverse::MAZE_FLOOR_HALFHEIGHT, screen->camera.position.z - 0.01f),
            glm::vec3(micrasverse::MAZE_FLOOR_HALFWIDTH, micrasverse::MAZE_FLOOR_HALFHEIGHT, 0.0f)
        );

        // Create ARGB scene objects
        for (auto& argb : this->simulationEngine->physicsEngine->getMicras().argb.argbs) {
            this->argbsSceneObjs.emplace_back(std::make_unique<ARGBRender>(argb.worldPosition, argb.size, argb.lightColorArray, argb.baseColorArray, argb.isOn, screen->camera));
        }

        // Create Lidar scene objects
        for (auto& lidar : this->simulationEngine->physicsEngine->getMicras().wallSensors.get_sensors()) {
            this->lidarsSceneObjs.emplace_back(std::make_unique<LidarRender>(lidar.rayMidPoint, lidar.rayDirection, lidar.reading, screen->camera));
        }
    }

    void RenderEngine::update() {
        screen->processInput();
        
        micrasRender->update(this->simulationEngine->physicsEngine->getMicras().getPosition(), this->simulationEngine->physicsEngine->getMicras().getRotation());
        
        
        for (size_t i = 0; i < this->simulationEngine->physicsEngine->getMicras().argb.argbs.size(); i++) {
            this->argbsSceneObjs[i]->update(this->simulationEngine->physicsEngine->getMicras().argb.argbs[i].worldPosition, this->simulationEngine->physicsEngine->getMicras().getRotation(), this->simulationEngine->physicsEngine->getMicras().argb.argbs[i].lightColorArray, this->simulationEngine->physicsEngine->getMicras().argb.argbs[i].isOn);
        }

        for (size_t i = 0; i < this->simulationEngine->physicsEngine->getMicras().wallSensors.get_sensors().size(); i++) {
            this->lidarsSceneObjs[i]->update(this->simulationEngine->physicsEngine->getMicras().wallSensors.get_sensors()[i].rayMidPoint, this->simulationEngine->physicsEngine->getMicras().wallSensors.get_sensors()[i].rayDirection, this->simulationEngine->physicsEngine->getMicras().wallSensors.get_sensors()[i].reading);
        }

        if(this->simulationEngine->wasReset) {
            this->reloadMaze(this->simulationEngine->getCurrentMazePath());
        }
    }

    void RenderEngine::renderFrame() {
        // Clear the screen color buffer
        screen->update(this->simulationEngine->physicsEngine->getMicras());
        
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
        screen->renderGUI(this->simulationEngine->physicsEngine->getMicras());

        //Swap front (screen) buffer and back (rendering) buffer + poll events like window resize, key and mouse input etc.
        screen->newFrame();      
    }

    void RenderEngine::reloadMaze(const std::string& mazeFilePath) {
            this->mazeRender->reloadElements(this->simulationEngine->physicsEngine->getMaze().getElements());
            this->simulationEngine->wasReset = false;
    }


} // namespace micrasverse::render

#endif // RENDER_ENGINE_CPP
