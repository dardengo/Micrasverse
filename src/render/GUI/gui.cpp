#include "GUI/gui.hpp"
#include "physics/box2d_micrasbody.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <filesystem>

namespace micrasverse::render {

GUI::GUI() : showStyleEditor(false), currentWindow(nullptr) {}

void GUI::setSimulationEngine(const std::shared_ptr<micrasverse::simulation::SimulationEngine>& simulationEngine) {
    this->simulationEngine = simulationEngine;
}

void GUI::init(GLFWwindow* window) {
    // Store the window for later use
    this->currentWindow = window;
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    
    // Configure ImGui for better performance (using available flags)
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    
    // Set smaller font size for better performance
    ImFontConfig fontConfig;
    fontConfig.SizePixels = 13.0f;
    fontConfig.OversampleH = 1;
    fontConfig.OversampleV = 1;
    fontConfig.PixelSnapH = true;
    
    // Memory optimization
    io.ConfigMemoryCompactTimer = 60.0f; // Compact memory less frequently
    
    this->plot.init();
    ImGui::StyleColorsDark();
    
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void GUI::update() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void GUI::draw(micrasverse::physics::Box2DMicrasBody& micrasBody) {
    // Check if we're in fullscreen mode for optimizations
    int width, height;
    glfwGetFramebufferSize(currentWindow, &width, &height);
    bool isLargeScreen = (width > 1600 || height > 900);
    
    // Main control panel
    ImGui::Begin("Micrasverse Control Panel");
    
    // Display FPS
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    
    // Display robot position and velocity
    ImGui::SeparatorText("Robot Status");
    ImGui::Text("Position: (%.2f, %.2f)", micrasBody.getPosition().x, micrasBody.getPosition().y);
    ImGui::Text("Rotation: %.2f", micrasBody.getAngle());
    ImGui::Text("Linear Velocity: (%.2f, %.2f)", micrasBody.getLinearVelocity().x, micrasBody.getLinearVelocity().y);
    
    // Robot Control Panel
    ImGui::SeparatorText("Robot Controls");
    
    // Movement Controls
    if (ImGui::TreeNode("Movement Controls")) {
        static float moveSpeed = 1.0f;
        ImGui::SliderFloat("Movement Speed", &moveSpeed, 0.1f, 2.0f);
        
        // Display WASD controls
        ImGui::Text("Use WASD keys to control the robot:");
        ImGui::BulletText("W - Forward");
        ImGui::BulletText("S - Backward");
        ImGui::BulletText("A - Turn Left");
        ImGui::BulletText("D - Turn Right");
        ImGui::BulletText("Space - Stop");
        
        // Display current movement state
        ImGui::Separator();
        ImGui::Text("Current Movement:");
        
        // Get keyboard state
        bool wPressed = ImGui::IsKeyDown(ImGuiKey_W);
        bool sPressed = ImGui::IsKeyDown(ImGuiKey_S);
        bool aPressed = ImGui::IsKeyDown(ImGuiKey_A);
        bool dPressed = ImGui::IsKeyDown(ImGuiKey_D);
        bool spacePressed = ImGui::IsKeyDown(ImGuiKey_Space);
        
        // Apply movement based on key state
        if (spacePressed) {
            micrasBody.getLocomotion().stop();
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "STOPPED");
        } else {
            // Forward/Backward
            if (wPressed) {
                micrasBody.getLocomotion().setCommand(moveSpeed, 0.0f);
                ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "FORWARD");
            } else if (sPressed) {
                micrasBody.getLocomotion().setCommand(-moveSpeed, 0.0f);
                ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "BACKWARD");
            } else {
                // No forward/backward movement
            }
            
            // Left/Right
            if (aPressed) {
                micrasBody.getLocomotion().setCommand(0.0f, moveSpeed);
                ImGui::TextColored(ImVec4(0.0f, 0.0f, 1.0f, 1.0f), "TURNING LEFT");
            } else if (dPressed) {
                micrasBody.getLocomotion().setCommand(0.0f, -moveSpeed);
                ImGui::TextColored(ImVec4(0.0f, 0.0f, 1.0f, 1.0f), "TURNING RIGHT");
            } else {
                // No turning
            }
            
            // If no keys are pressed, stop the robot
            if (!wPressed && !sPressed && !aPressed && !dPressed) {
                micrasBody.getLocomotion().stop();
                ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "IDLE");
            }
        }
        
        ImGui::TreePop();
    }
    
    // Sensor Controls
    if (ImGui::TreeNode("Sensor Controls")) {
        // Wall Sensors Status
        ImGui::Text("Wall Sensors:");
        auto& wallSensors = micrasBody.getWallSensors();
        for (size_t i = 0; i < 4; ++i) {
            float reading = wallSensors.get_sensors()[i].getReadingVisual();
            ImGui::ProgressBar(reading, ImVec2(-1, 0), 
                             ("Sensor " + std::to_string(i) + ": " + std::to_string(reading)).c_str());
        }
        
        ImGui::TreePop();
    }
    
    // LED Controls
    if (ImGui::TreeNode("LED Controls")) {
        auto& argb = micrasBody.getArgb();
        static std::array<float, 3> color = argb.argbs[0].getLightColorArray();
        if (ImGui::ColorEdit3("LED Color", color.data())) {
            // Update LED color when changed
            micrasverse::types::Color newColor{color[0], color[1], color[2]};
            argb.argbs[0].setColor(newColor);
        }
        
        // LED On/Off toggle
        bool isLedOn = argb.argbs[0].isOn();
        if (ImGui::Checkbox("LED On/Off", &isLedOn)) {
            if (!isLedOn) {
                argb.argbs[0].turnOff();
            } else {
                // Turn on with current color
                micrasverse::types::Color currentColor{color[0], color[1], color[2]};
                argb.argbs[0].turnOn(currentColor);
            }
        }
        
        ImGui::TreePop();
    }
    
    // DIP Switch Controls
    if (ImGui::TreeNode("DIP Switches")) {
        auto& dipSwitch = micrasBody.getDipSwitch();
        for (size_t i = 0; i < 4; ++i) {
            bool switchState = dipSwitch.get_switch_state(i);
            if (ImGui::Checkbox(("Switch " + std::to_string(i)).c_str(), &switchState)) {
                dipSwitch.set_switch_state(i, switchState);
            }
        }
        
        ImGui::TreePop();
    }
    
    // Micras Button Press Settings
    if (ImGui::TreeNode("Micras Button Press Settings")) {
        ImGui::Text("Press duration settings for Micras buttons:");
        
        // Get the button from the micras body
        auto& button = micrasBody.getButton();
        
        // Short press button
        if (ImGui::Button("Short Press", ImVec2(120, 30))) {
            // Simulate a short press
            button.set_state(true);
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Short press activated");
        }
        
        ImGui::SameLine();
        ImGui::Text("Duration: 0.5 seconds");
        
        // Long press button
        if (ImGui::Button("Long Press", ImVec2(120, 30))) {
            // Simulate a long press
            button.set_state(true);
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Long press activated");
        }
        
        ImGui::SameLine();
        ImGui::Text("Duration: 1.5 seconds");
        
        // Extra long press button
        if (ImGui::Button("Extra Long Press", ImVec2(120, 30))) {
            // Simulate an extra long press
            button.set_state(true);
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Extra long press activated");
        }
        
        ImGui::SameLine();
        ImGui::Text("Duration: 3.0 seconds");
        
        // Display current button state
        ImGui::Separator();
        ImGui::Text("Current Button State:");
        
        // Get the current button status
        auto status = button.get_status();
        std::string statusText;
        ImVec4 statusColor;
        
        switch (status) {
            case micras::proxy::Button::Status::NO_PRESS:
                statusText = "No button pressed";
                statusColor = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
                break;
            case micras::proxy::Button::Status::SHORT_PRESS:
                statusText = "Short press";
                statusColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
                break;
            case micras::proxy::Button::Status::LONG_PRESS:
                statusText = "Long press";
                statusColor = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
                break;
            case micras::proxy::Button::Status::EXTRA_LONG_PRESS:
                statusText = "Extra long press";
                statusColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
                break;
            default:
                statusText = "Unknown";
                statusColor = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
                break;
        }
        
        ImGui::TextColored(statusColor, "%s", statusText.c_str());
        
        // Add a button to release the button state
        if (button.is_pressed()) {
            if (ImGui::Button("Release Button", ImVec2(120, 30))) {
                button.set_state(false);
            }
        }
        
        ImGui::TreePop();
    }
    
    // Display robot controller information if available
    if (this->simulationEngine) {
        // Display wall sensor readings
        ImGui::SeparatorText("Wall Sensors");
        for (size_t i = 0; i < 4; ++i) {
            ImGui::Text("Sensor %zu: %.2f", i, micrasBody.getWallSensors().get_sensors()[i].getReadingVisual());
        }
    }

    // Simulation controls
    if (this->simulationEngine) {
        ImGui::SeparatorText("Simulation Controls");
    
        const auto& mazePaths = simulationEngine->getMazePaths();
        static int selectedMazeIdx = 0;
    
        if (!mazePaths.empty()) {
            // Only use filenames for the dropdown
            std::string currentMazeFile = std::filesystem::path(mazePaths[selectedMazeIdx]).filename().string();
        
            if (ImGui::BeginCombo("Maze", currentMazeFile.c_str())) {
                for (int i = 0; i < mazePaths.size(); ++i) {
                    std::string label = std::filesystem::path(mazePaths[i]).filename().string();
                    bool selected = (selectedMazeIdx == i);
                    if (ImGui::Selectable(label.c_str(), selected)) {
                        selectedMazeIdx = i;
                    }
                    if (selected) ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
        
            // Restart simulation with selected maze
            
            if (ImGui::Button("Load Maze")) {
                simulationEngine->resetSimulation(mazePaths[selectedMazeIdx]);
            }

            if (ImGui::Button("Reset Simulation"))
            {
                simulationEngine->resetSimulation();
            }
            
        } else {
            ImGui::TextColored(ImVec4(1,0.5f,0.5f,1), "No maze files found!");
        }
    
        // Toggle simulation running
        if (ImGui::Button(simulationEngine->isPaused ? "Start" : "Pause")) {
            simulationEngine->togglePause();
        }
    
        ImGui::SameLine();
    
        // Step one frame
        if (ImGui::Button("Step")) {
            simulationEngine->stepThroughSimulation();
        }
    
        ImGui::Text("Simulation is %s", simulationEngine->isPaused ? "Paused" : "Running");
    }
    
    ImGui::Text("Micras position: (%.2f, %.2f)", micrasBody.getPosition().x, micrasBody.getPosition().y);
    ImGui::Text("Micras linear velocity: (%.2f, %.2f)", micrasBody.getLinearVelocity().x, micrasBody.getLinearVelocity().y);
    ImGui::Text("FAN is: %.2f", micrasBody.getDipSwitch().get_switch_state(0) ? 1.0f : 0.0f); ImGui::SameLine();
    
    // Skip color editor in fullscreen mode to improve performance
    if (!isLargeScreen) {
        std::array<float, 3> color = micrasBody.getArgb().argbs[0].getLightColorArray();
        ImGui::ColorEdit3("LED color", color.data());
    }

    // Option to show style editor (defaults to off)
    ImGui::Checkbox("Show Style Editor", &showStyleEditor);

    // Only draw plots if not in fullscreen mode or if user explicitly enabled them
    if (!isLargeScreen || (isLargeScreen && this->plot.showPlots)) {
        this->plot.draw(micrasBody);
    } else {
        // Just show checkbox to enable plots when in fullscreen
        ImGui::Checkbox("Show Performance Plots", &this->plot.showPlots);
    }
    
    ImGui::End();

    // Only render style editor when enabled - but never in fullscreen
    if (showStyleEditor && !isLargeScreen) {
        ImGui::Begin("Style Editor");
        ImGui::ShowStyleEditor();
        ImGui::End();
    }
}

void GUI::render() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::destroy() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    this->plot.destroy();
    ImGui::DestroyContext();
    this->currentWindow = nullptr;
}

} // namespace micrasverse::render
