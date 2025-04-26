#ifndef RENDER_ENGINE_CPP
#define RENDER_ENGINE_CPP

#include "render/render_engine.hpp"
#include "render/camera.hpp"
#include "render/screen.hpp"
#include "physics/box2d_distance_sensor.hpp"
#include "physics/argb.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
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
        
        // Create ARGB scene objects
        auto& argb0 = this->simulationEngine->physicsEngine->getMicras().getArgb(0);
        this->argbsSceneObjs.emplace_back(std::make_unique<ARGBRender>(
            b2Vec2(argb0.getWorldPosition().x, argb0.getWorldPosition().y),
            b2Vec2(argb0.getSize().x, argb0.getSize().y),
            argb0.getLightColorArray(),
            argb0.getBaseColorArray(),
            argb0.isOn(),
            screen->camera
        ));
        
        try {
            auto& argb1 = this->simulationEngine->physicsEngine->getMicras().getArgb(1);
            this->argbsSceneObjs.emplace_back(std::make_unique<ARGBRender>(
                b2Vec2(argb1.getWorldPosition().x, argb1.getWorldPosition().y),
                b2Vec2(argb1.getSize().x, argb1.getSize().y),
                argb1.getLightColorArray(),
                argb1.getBaseColorArray(),
                argb1.isOn(),
                screen->camera
            ));
        } catch (const std::exception& e) {
            // If there's no second ARGB, just continue
        }

        // Create Lidar scene objects
        for (size_t i = 0; i < 4; i++) {
            auto& lidar = this->simulationEngine->physicsEngine->getMicras().getDistanceSensor(i);
            this->lidarsSceneObjs.emplace_back(std::make_unique<LidarRender>(
                b2Vec2(lidar.getRayMidPoint().x, lidar.getRayMidPoint().y),
                b2MakeRot(lidar.getAngle()),
                lidar.getReadingVisual(),
                screen->camera
            ));
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
        isFullscreen = (width > 1919 || height > 1079);
        
        // Update Micras rendering position based on physics
        micrasRender->update(
            this->simulationEngine->physicsEngine->getMicras().getPosition(), 
            this->simulationEngine->physicsEngine->getMicras().getRotation()
        );
        
        // Update ARGB LEDs - use batch update for fullscreen optimization
        if (isFullscreen) {
            // Batch update ARGBs only if robot is in view
            if (isObjectInView(micrasRender->getBoundingBox())) {
                auto& argb = this->simulationEngine->physicsEngine->getMicras().getArgb(0);
                this->argbsSceneObjs[0]->update(
                    b2Vec2(argb.getWorldPosition().x, argb.getWorldPosition().y),
                    this->simulationEngine->physicsEngine->getMicras().getRotation(),
                    argb.getLightColorArray(),
                    argb.isOn()
                );
                if (this->argbsSceneObjs.size() > 1) {
                    auto& argb1 = this->simulationEngine->physicsEngine->getMicras().getArgb(1);
                    this->argbsSceneObjs[1]->update(
                        b2Vec2(argb1.getWorldPosition().x, argb1.getWorldPosition().y),
                        this->simulationEngine->physicsEngine->getMicras().getRotation(),
                        argb1.getLightColorArray(),
                        argb1.isOn()
                    );
                }
            }
        } else {
            // Standard update in windowed mode
            auto& argb = this->simulationEngine->physicsEngine->getMicras().getArgb(0);
            this->argbsSceneObjs[0]->update(
                b2Vec2(argb.getWorldPosition().x, argb.getWorldPosition().y),
                this->simulationEngine->physicsEngine->getMicras().getRotation(),
                argb.getLightColorArray(),
                argb.isOn()
            );
            if (this->argbsSceneObjs.size() > 1) {
                auto& argb1 = this->simulationEngine->physicsEngine->getMicras().getArgb(1);
                this->argbsSceneObjs[1]->update(
                    b2Vec2(argb1.getWorldPosition().x, argb1.getWorldPosition().y),
                    this->simulationEngine->physicsEngine->getMicras().getRotation(),
                    argb1.getLightColorArray(),
                    argb1.isOn()
                );
            }
        }

        // Update distance sensors - with optimization for fullscreen
        if (isFullscreen) {
            // Only update visible sensors
            int visibleCount = 0;
            for (size_t i = 0; i < 4; i++) {
                // Skip updating if too many visible sensors already (optimization)
                if (visibleCount >= 4 && i > 0) continue;
                
                // Check reading to determine visibility
                float reading = this->simulationEngine->physicsEngine->getMicras().getDistanceSensor(i).getReadingVisual();
                if (reading > 0.005f) {
                    visibleCount++;
                    this->lidarsSceneObjs[i]->update(
                        b2Vec2(this->simulationEngine->physicsEngine->getMicras().getDistanceSensor(i).getRayMidPoint().x, this->simulationEngine->physicsEngine->getMicras().getDistanceSensor(i).getRayMidPoint().y),
                        b2MakeRot(std::atan2(this->simulationEngine->physicsEngine->getMicras().getDistanceSensor(i).getRayDirection().y, 
                                           this->simulationEngine->physicsEngine->getMicras().getDistanceSensor(i).getRayDirection().x)),
                        reading
                    );
                } else {
                    // Force invisible for better performance
                    this->lidarsSceneObjs[i]->update(
                        b2Vec2(this->simulationEngine->physicsEngine->getMicras().getDistanceSensor(i).getRayMidPoint().x, this->simulationEngine->physicsEngine->getMicras().getDistanceSensor(i).getRayMidPoint().y),
                        b2MakeRot(std::atan2(this->simulationEngine->physicsEngine->getMicras().getDistanceSensor(i).getRayDirection().y,
                                           this->simulationEngine->physicsEngine->getMicras().getDistanceSensor(i).getRayDirection().x)),
                        0.0f
                    );
                }
            }
        } else {
            // Standard update in windowed mode
            for (size_t i = 0; i < 4; i++) {
                auto& lidar = this->simulationEngine->physicsEngine->getMicras().getDistanceSensor(i);
                this->lidarsSceneObjs[i]->update(
                    b2Vec2(lidar.getRayMidPoint().x, lidar.getRayMidPoint().y),
                    b2MakeRot(std::atan2(lidar.getRayDirection().y,
                                       lidar.getRayDirection().x)),
                    lidar.getReadingVisual()
                );
            }
        }

        // Update firmware walls if proxy bridge exists
        if (proxyBridge) {
            mazeRender->updateFirmwareWalls(proxyBridge);
        }

        // Reload maze if needed
        if(this->simulationEngine->wasReset) {
            this->reloadMaze(this->simulationEngine->getCurrentMazePath());
            
            // Force all lidar rays to be invisible after reset
            for (auto& lidarObj : this->lidarsSceneObjs) {
                lidarObj->update(
                    {0, 0},  // Position doesn't matter since they'll be invisible
                    b2MakeRot(0.0f),  // Direction doesn't matter either
                    0.0f     // Zero reading ensures they're invisible
                );
            }
        }
    }

    void RenderEngine::setProxyBridge(const std::shared_ptr<micras::ProxyBridge>& proxyBridge) {
    this->proxyBridge = proxyBridge;
    
    // Initial update of firmware walls
    if (mazeRender) {
            mazeRender->updateFirmwareWalls(proxyBridge);
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
        
        // Create a temporary light for rendering purposes
        glm::vec3 lightPosition = glm::vec3(micrasverse::MAZE_FLOOR_HALFWIDTH, micrasverse::MAZE_FLOOR_HALFHEIGHT, screen->camera.position.z - 0.01f);
        glm::vec3 lightAmbient = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 lightDiffuse = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 lightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
        
        // 1. Render maze (floor and walls)
        mazeRender->render(
            screen->view, 
            screen->projection, 
            lightPosition, 
            screen->camera.position, 
            lightAmbient, 
            lightDiffuse, 
            lightSpecular
        );
        
        // 2. Render Micras robot (uses same shader as maze)
        micrasRender->render(
            screen->view, 
            screen->projection, 
            lightPosition, 
            screen->camera.position, 
            lightAmbient, 
            lightDiffuse, 
            lightSpecular
        );

        // Now render everything using the light shader
        // Check if any lidars are visible
        bool anyLidarsVisible = false;
        int visibleLidarCount = 0;
        
        for (auto& lidarSceneObj: lidarsSceneObjs) {
            if (lidarSceneObj->isVisible()) {
                anyLidarsVisible = true;
                visibleLidarCount++;
                
                // For fullscreen, limit the number of lidar visualizations for performance
                if (isFullscreen && visibleLidarCount > 4) {
                    lidarSceneObj->setVisible(false);
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
                if (lidarSceneObj->isVisible()) {
                    lidarSceneObj->render(screen->view, screen->projection);
                }
            }
        }

        // 6. Render GUI (ImGui) on top of everything
        screen->renderGUI(this->simulationEngine->physicsEngine->getMicras());

        // Swap buffers and poll events
        screen->newFrame();      
    }

    void RenderEngine::reloadMaze(const std::string& mazeFilePath) {
        this->mazeRender->reloadElements(this->simulationEngine->physicsEngine->getMaze().getElements());
        
        // Update ARGB render positions after reset
        auto& argb = this->simulationEngine->physicsEngine->getMicras().getArgb(0);
        this->argbsSceneObjs[0]->update(
            b2Vec2(argb.getWorldPosition().x, argb.getWorldPosition().y),
            this->simulationEngine->physicsEngine->getMicras().getRotation(),
            argb.getLightColorArray(),
            argb.isOn()
        );
        if (this->argbsSceneObjs.size() > 1) {
            auto& argb1 = this->simulationEngine->physicsEngine->getMicras().getArgb(1);
            this->argbsSceneObjs[1]->update(
                b2Vec2(argb1.getWorldPosition().x, argb1.getWorldPosition().y),
                this->simulationEngine->physicsEngine->getMicras().getRotation(),
                argb1.getLightColorArray(),
                argb1.isOn()
            );
        }
        
        // Update Lidar render positions after reset
        for (size_t i = 0; i < 4; i++) {
            auto& lidar = this->simulationEngine->physicsEngine->getMicras().getDistanceSensor(i);
            this->lidarsSceneObjs[i]->update(
                b2Vec2(lidar.getRayMidPoint().x, lidar.getRayMidPoint().y),
                b2MakeRot(std::atan2(lidar.getRayDirection().y,
                                   lidar.getRayDirection().x)),
                lidar.getReadingVisual()
            );
        }
        
        this->simulationEngine->wasReset = false;
    }


} // namespace micrasverse::render

#endif // RENDER_ENGINE_CPP
