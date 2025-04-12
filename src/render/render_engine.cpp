#ifndef RENDER_ENGINE_CPP
#define RENDER_ENGINE_CPP

#include "render/render_engine.hpp"
#include <iostream>
#include <algorithm>

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

        // Initialize light shaders
        this->lightShader = std::make_unique<Shader>();
        this->lightShader->generate("src/render/assets/vertex-core.glsl", "src/render/assets/fragment-led.glsl");
        
        // Initialize maze rendering
        this->mazeRender = std::make_unique<MazeRender>(this->simulationEngine->physicsEngine->getMaze().getElements());
        this->mazeRender->init();
        
        // Initialize micras rendering
        this->micrasRender = std::make_unique<MicrasRender>(
            this->simulationEngine->physicsEngine->getMicras().getPosition(),
            this->simulationEngine->physicsEngine->getMicras().getSize()
        );
        
        // Create light
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
            this->argbsSceneObjs.emplace_back(std::make_unique<ARGBRender>(argb.worldPosition, argb.size, argb.lightColorArray, argb.baseColorArray, argb.isOn(), screen->camera));
        }

        // Create Lidar scene objects
        for (auto& lidar : this->simulationEngine->physicsEngine->getMicras().wallSensors.get_sensors()) {
            this->lidarsSceneObjs.emplace_back(std::make_unique<LidarRender>(lidar.rayMidPoint, lidar.rayDirection, lidar.reading, screen->camera));
        }
        
        // Initialize optimization flags
        isFullscreen = false;
        frustumCullingEnabled = true;
    }

    void RenderEngine::update() {
        // Process input & update simulation
        screen->processInput();
        
        // Check if we're in fullscreen mode for optimizations
        int width, height;
        glfwGetFramebufferSize(screen->getWindow(), &width, &height);
        isFullscreen = (width > 1600 || height > 900);
        
        // Update Micras rendering position based on physics
        micrasRender->update(
            this->simulationEngine->physicsEngine->getMicras().getPosition(), 
            this->simulationEngine->physicsEngine->getMicras().getRotation()
        );
        
        // Update ARGB LEDs - use batch update for fullscreen optimization
        if (isFullscreen) {
            // Batch update ARGBs only if robot is in view
            if (isObjectInView(micrasRender->getBoundingBox())) {
                for (size_t i = 0; i < this->simulationEngine->physicsEngine->getMicras().argb.argbs.size(); i++) {
                    this->argbsSceneObjs[i]->update(
                        this->simulationEngine->physicsEngine->getMicras().argb.argbs[i].worldPosition, 
                        this->simulationEngine->physicsEngine->getMicras().getRotation(), 
                        this->simulationEngine->physicsEngine->getMicras().argb.argbs[i].lightColorArray, 
                        this->simulationEngine->physicsEngine->getMicras().argb.argbs[i].isOn()
                    );
                }
            }
        } else {
            // Standard update in windowed mode
            for (size_t i = 0; i < this->simulationEngine->physicsEngine->getMicras().argb.argbs.size(); i++) {
                this->argbsSceneObjs[i]->update(
                    this->simulationEngine->physicsEngine->getMicras().argb.argbs[i].worldPosition, 
                    this->simulationEngine->physicsEngine->getMicras().getRotation(), 
                    this->simulationEngine->physicsEngine->getMicras().argb.argbs[i].lightColorArray, 
                    this->simulationEngine->physicsEngine->getMicras().argb.argbs[i].isOn()
                );
            }
        }

        // Update distance sensors - with optimization for fullscreen
        if (isFullscreen) {
            // Only update visible sensors
            int visibleCount = 0;
            for (size_t i = 0; i < this->simulationEngine->physicsEngine->getMicras().wallSensors.get_sensors().size(); i++) {
                // Skip updating if too many visible sensors already (optimization)
                if (visibleCount >= 4 && i > 0) continue;
                
                // Check reading to determine visibility
                float reading = this->simulationEngine->physicsEngine->getMicras().wallSensors.get_sensors()[i].reading;
                if (reading > 0.1f) {
                    visibleCount++;
                    this->lidarsSceneObjs[i]->update(
                        this->simulationEngine->physicsEngine->getMicras().wallSensors.get_sensors()[i].rayMidPoint, 
                        this->simulationEngine->physicsEngine->getMicras().wallSensors.get_sensors()[i].rayDirection, 
                        reading
                    );
                } else {
                    // Force invisible for better performance
                    this->lidarsSceneObjs[i]->update(
                        this->simulationEngine->physicsEngine->getMicras().wallSensors.get_sensors()[i].rayMidPoint, 
                        this->simulationEngine->physicsEngine->getMicras().wallSensors.get_sensors()[i].rayDirection, 
                        0.0f
                    );
                }
            }
        } else {
            // Standard update in windowed mode
            for (size_t i = 0; i < this->simulationEngine->physicsEngine->getMicras().wallSensors.get_sensors().size(); i++) {
                this->lidarsSceneObjs[i]->update(
                    this->simulationEngine->physicsEngine->getMicras().wallSensors.get_sensors()[i].rayMidPoint, 
                    this->simulationEngine->physicsEngine->getMicras().wallSensors.get_sensors()[i].rayDirection, 
                    this->simulationEngine->physicsEngine->getMicras().wallSensors.get_sensors()[i].reading
                );
            }
        }

        // Reload maze if needed
        if(this->simulationEngine->wasReset) {
            this->reloadMaze(this->simulationEngine->getCurrentMazePath());
            
            // Force all lidar rays to be invisible after reset
            for (auto& lidarObj : this->lidarsSceneObjs) {
                lidarObj->update(
                    {0, 0},  // Position doesn't matter since they'll be invisible
                    {0, 0},  // Direction doesn't matter either
                    0.0f     // Zero reading ensures they're invisible
                );
            }
        }
    }
    
    bool RenderEngine::isObjectInView(const BoundingBox& bbox) {
        if (!frustumCullingEnabled) {
            return true;
        }
        
        // Simple frustum culling check
        glm::mat4 viewProj = screen->projection * screen->view;
        
        // Transform all 8 corners of the bounding box
        std::array<glm::vec4, 8> corners = {
            glm::vec4(bbox.min.x, bbox.min.y, bbox.min.z, 1.0f),
            glm::vec4(bbox.max.x, bbox.min.y, bbox.min.z, 1.0f),
            glm::vec4(bbox.min.x, bbox.max.y, bbox.min.z, 1.0f),
            glm::vec4(bbox.max.x, bbox.max.y, bbox.min.z, 1.0f),
            glm::vec4(bbox.min.x, bbox.min.y, bbox.max.z, 1.0f),
            glm::vec4(bbox.max.x, bbox.min.y, bbox.max.z, 1.0f),
            glm::vec4(bbox.min.x, bbox.max.y, bbox.max.z, 1.0f),
            glm::vec4(bbox.max.x, bbox.max.y, bbox.max.z, 1.0f)
        };
        
        // Check if any corner is inside the view frustum
        for (auto& corner : corners) {
            glm::vec4 clipSpace = viewProj * corner;
            
            // If w is negative, the point is behind the camera
            if (clipSpace.w <= 0) continue;
            
            // Normalize to get NDC coordinates
            glm::vec3 ndc = glm::vec3(clipSpace) / clipSpace.w;
            
            // Check if inside NDC cube (-1 to 1)
            if (ndc.x >= -1.0f && ndc.x <= 1.0f &&
                ndc.y >= -1.0f && ndc.y <= 1.0f && 
                ndc.z >= -1.0f && ndc.z <= 1.0f) {
                return true;
            }
        }
        
        return false;
    }

    void RenderEngine::renderFrame() {
        // Update screen (clear buffers, update matrices)
        screen->update(this->simulationEngine->physicsEngine->getMicras());
        
        // Performance optimizations for fullscreen
        if (isFullscreen) {
            glDisable(GL_DITHER);
        } else {
            glEnable(GL_DITHER);
        }
        
        // Group rendering by shader to minimize state changes (big performance win)
        // First render everything using the standard shader
        
        // Use the maze shader for standard objects 
        mazeRender->shader.activate(screen->view, screen->projection);
        
        // 1. Render maze (floor and walls)
        mazeRender->render(
            screen->view, 
            screen->projection, 
            light->position, 
            screen->camera.position, 
            light->ambient, 
            light->diffuse, 
            light->specular
        );
        
        // 2. Render Micras robot (uses same shader as maze)
        micrasRender->render(
            screen->view, 
            screen->projection, 
            light->position, 
            screen->camera.position, 
            light->ambient, 
            light->diffuse, 
            light->specular
        );

        // Now render everything using the light shader
        // Check if any lidars are visible
        bool anyLidarsVisible = false;
        int visibleLidarCount = 0;
        
        for (auto& lidarSceneObj: lidarsSceneObjs) {
            if (lidarSceneObj->visible) {
                anyLidarsVisible = true;
                visibleLidarCount++;
                
                // For fullscreen, limit the number of lidar visualizations for performance
                if (isFullscreen && visibleLidarCount > 4) {
                    lidarSceneObj->visible = false;
                }
            }
        }
        
        // Only activate light shader if needed
        if (anyLidarsVisible || !argbsSceneObjs.empty()) {
            lightShader->activate(screen->view, screen->projection);
            
            // 3. Render ARGBs (LEDs)
            for (auto& argbSceneObj: argbsSceneObjs) {
                argbSceneObj->render(screen->view, screen->projection);
            }

            // 4. Render distance sensor visualizations
            for (auto& lidarSceneObj: lidarsSceneObjs) {
                if (lidarSceneObj->visible) {
                    lidarSceneObj->render(screen->view, screen->projection);
                }
            }

            // 5. Render light
            light->render(*lightShader, true);
        }

        // 6. Render GUI (ImGui) on top of everything
        screen->renderGUI(this->simulationEngine->physicsEngine->getMicras());

        // Swap buffers and poll events
        screen->newFrame();      
    }

    void RenderEngine::reloadMaze(const std::string& mazeFilePath) {
        this->mazeRender->reloadElements(this->simulationEngine->physicsEngine->getMaze().getElements());
        this->simulationEngine->wasReset = false;
    }


} // namespace micrasverse::render

#endif // RENDER_ENGINE_CPP
