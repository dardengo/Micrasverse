#include "GUI/gui.hpp"
#include "physics/box2d_micrasbody.hpp"
#include "micras/proxy/proxy_bridge.hpp"
#include "physics/box2d_dipswitch.hpp"
#include "physics/box2d_motor.hpp"
#include "io/keyboard.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <filesystem>
#include <thread>
#include <chrono>

namespace micrasverse::render {

GUI::GUI() : showStyleEditor(false), currentWindow(nullptr), proxyBridge(nullptr) {}

void GUI::setSimulationEngine(const std::shared_ptr<micrasverse::simulation::SimulationEngine>& simulationEngine) {
    this->simulationEngine = simulationEngine;
}

void GUI::setRenderEngine(RenderEngine* renderEngine) {
    this->renderEngine = renderEngine;
}

void GUI::setProxyBridge(const std::shared_ptr<micras::ProxyBridge>& proxyBridge) {
    this->proxyBridge = proxyBridge;
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
    if (!proxyBridge) {
        ImGui::Begin("Error");
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Proxy Bridge not initialized!");
        ImGui::End();
        return;
    }

    // Check if we're in fullscreen mode for optimizations
    int width, height;
    glfwGetFramebufferSize(currentWindow, &width, &height);
    bool isLargeScreen = (width > 1919 || height > 1079);
    
    // Main control panel
    ImGui::Begin("Micrasverse Control Panel");
    
    /// Toggle simulation running
        if (ImGui::Button(simulationEngine->isPaused ? "Start" : "Pause")) {
            simulationEngine->togglePause();
        }
    
        ImGui::SameLine();
    
        // Step one frame
        if (ImGui::Button("Step")) {
            simulationEngine->stepThroughSimulation();
        }

        ImGui::SameLine();
    
        ImGui::Text("Simulation is %s", simulationEngine->isPaused ? "Paused" : "Running");
    
    // Robot Status Section
    if (ImGui::CollapsingHeader("Robot Status", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Text("Micras Body Pose: (%.2f, %.2f, %.2f)", micrasBody.getPosition().x-WALL_THICKNESS, micrasBody.getPosition().y-WALL_THICKNESS, micrasBody.getAngle() + B2_PI/2.0f);
        auto pose = proxyBridge->get_current_pose();
        ImGui::Text("Micras Controller Pose: (%.2f, %.2f, %.2f)", pose.position.x, pose.position.y, pose.orientation);
        ImGui::Separator();
        auto goal = proxyBridge->get_current_goal();
        ImGui::Text("Micras Controller Goal: (%.2f, %.2f)", goal.x, goal.y);
        ImGui::Separator();
        ImGui::Text("Micras Body Linear Velocity: (%.2f, %.2f)", micrasBody.getLinearVelocity().x, micrasBody.getLinearVelocity().y);

        // Add MicrasController objective and current_action
        if (proxyBridge) {
            ImGui::Separator();
            ImGui::Text("MicrasController Status:");
            
            // Display objective with color coding
            std::string objectiveText = "Objective: " + proxyBridge->get_objective_string();
            ImVec4 objectiveColor;
            
            switch (proxyBridge->get_objective()) {
                case micras::core::Objective::EXPLORE:
                    objectiveColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f); // Green
                    break;
                case micras::core::Objective::RETURN:
                    objectiveColor = ImVec4(0.0f, 0.0f, 1.0f, 1.0f); // Blue
                    break;
                case micras::core::Objective::SOLVE:
                    objectiveColor = ImVec4(1.0f, 0.5f, 0.0f, 1.0f); // Orange
                    break;
                default:
                    objectiveColor = ImVec4(0.5f, 0.5f, 0.5f, 1.0f); // Gray
                    break;
            }
            
            ImGui::PushStyleColor(ImGuiCol_Text, objectiveColor);
            ImGui::Text("%s", objectiveText.c_str());
            ImGui::PopStyleColor();
            
            // Display current action with color coding
            std::string actionText = "Current Action: " + proxyBridge->get_action_type_string();
            ImVec4 actionColor = ImVec4(0.5f, 0.5f, 0.5f, 1.0f); // Default gray
            
            // Use simpler approach since we don't have direct access to the Action type
            ImGui::PushStyleColor(ImGuiCol_Text, actionColor);
            ImGui::Text("%s", actionText.c_str());
            ImGui::PopStyleColor();
            
        }
    }
    
    // Movement Controls Section
    if (ImGui::CollapsingHeader("Movement Controls", ImGuiTreeNodeFlags_DefaultOpen)) {
        static float moveSpeed = 5.0f;
        ImGui::SliderFloat("Movement Speed", &moveSpeed, 0.1f, 10.0f);
        
        // Display WASD controls in a more compact way
        ImGui::Text("WASD Controls:");
        ImGui::Columns(2, "controls", false);
        ImGui::Text("W - Forward");
        ImGui::NextColumn();
        ImGui::Text("S - Backward");
        ImGui::NextColumn();
        ImGui::Text("A - Turn Left");
        ImGui::NextColumn();
        ImGui::Text("D - Turn Right");
        ImGui::NextColumn();
        ImGui::Text("Space - Stop");
        ImGui::Columns(1);
        
        // Display current movement state
        ImGui::Separator();
        ImGui::Text("Current Movement:");
        
        // Get keyboard state using micrasverse::io::Keyboard
        bool wPressed = micrasverse::io::Keyboard::key(GLFW_KEY_W);
        bool sPressed = micrasverse::io::Keyboard::key(GLFW_KEY_S);
        bool aPressed = micrasverse::io::Keyboard::key(GLFW_KEY_A);
        bool dPressed = micrasverse::io::Keyboard::key(GLFW_KEY_D);
        bool spacePressed = micrasverse::io::Keyboard::key(GLFW_KEY_SPACE);
        
        // Check for key up events to set commands to zero
        bool wReleased = micrasverse::io::Keyboard::keyWentUp(GLFW_KEY_W);
        bool sReleased = micrasverse::io::Keyboard::keyWentUp(GLFW_KEY_S);
        bool aReleased = micrasverse::io::Keyboard::keyWentUp(GLFW_KEY_A);
        bool dReleased = micrasverse::io::Keyboard::keyWentUp(GLFW_KEY_D);
        
        // Track current command values
        static float currentLinear = 0.0f;
        static float currentAngular = 0.0f;
        
        // Apply movement based on key state
        if (spacePressed) {
            proxyBridge->stop_motors();
            proxyBridge->disable_motors();
            currentLinear = 0.0f;
            currentAngular = 0.0f;
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "STOPPED");
        } else {
            // Clear previous commands and enable motors if any movement key is pressed
            if (wPressed || sPressed || aPressed || dPressed) {
                proxyBridge->enable_motors();
                
                // Forward/Backward movement
                float linearSpeed = 0.0f;
                if (wPressed) linearSpeed = moveSpeed;
                if (sPressed) linearSpeed = -moveSpeed;
                
                // Left/Right turning
                float angularSpeed = 0.0f;
                if (aPressed) angularSpeed = moveSpeed;
                if (dPressed) angularSpeed = -moveSpeed;
                
                // Update tracked values
                currentLinear = linearSpeed;
                currentAngular = angularSpeed;
                
                // Apply the combined command
                proxyBridge->set_command(linearSpeed, angularSpeed);
                
                // Display movement status
                if (linearSpeed > 0) {
                    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "FORWARD");
                } else if (linearSpeed < 0) {
                    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "BACKWARD");
                }
                
                if (angularSpeed > 0) {
                    ImGui::TextColored(ImVec4(0.0f, 0.0f, 1.0f, 1.0f), "TURNING LEFT");
                } else if (angularSpeed < 0) {
                    ImGui::TextColored(ImVec4(0.0f, 0.0f, 1.0f, 1.0f), "TURNING RIGHT");
                }
            } else {
                // Handle key releases - set command to 0
                if (wReleased || sReleased) {
                    currentLinear = 0.0f;
                    proxyBridge->set_command(0.0f, currentAngular);
                }
                
                if (aReleased || dReleased) {
                    currentAngular = 0.0f;
                    proxyBridge->set_command(currentLinear, 0.0f);
                }
                
                // No keys pressed
                if (!wPressed && !sPressed && !aPressed && !dPressed) {
                    ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "IDLE");
                }
            }
        }
    }
    
    // DIP Switch Controls Section
    if (ImGui::CollapsingHeader("DIP Switches", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Columns(4, "dipswitches", false);
        
        for (size_t i = 0; i < 4; ++i) {
            bool switchState = proxyBridge->get_dip_switch_state(i);
            if (ImGui::Checkbox(("Switch " + std::to_string(i)).c_str(), &switchState)) {
                // Note: DIP switches are read-only through the proxy bridge
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "DIP switches are read-only");
            }
            ImGui::NextColumn();
        }
        ImGui::Columns(1);
    }
    
    // Button Controls Section
    if (ImGui::CollapsingHeader("Button Controls", ImGuiTreeNodeFlags_DefaultOpen)) {
        // Get the current button status
        auto status = proxyBridge->get_button_status();
        std::string statusText;
        ImVec4 statusColor;
        
        // Check if a button timer is active and handle timeout
        if (buttonTimerActive) {
            auto currentTime = std::chrono::steady_clock::now();
            float elapsedTime = std::chrono::duration<float>(currentTime - buttonActivationTime).count();
            
            int durationIndex = 0;
            switch (status) {
                case micras::proxy::Button::Status::SHORT_PRESS:
                    durationIndex = 0;
                    break;
                case micras::proxy::Button::Status::LONG_PRESS:
                    durationIndex = 1;
                    break;
                case micras::proxy::Button::Status::EXTRA_LONG_PRESS:
                    durationIndex = 2;
                    break;
                default:
                    buttonTimerActive = false;
                    break;
            }
            
            // Reset the button status after the configured duration
            if (status != micras::proxy::Button::Status::NO_PRESS && 
                elapsedTime >= buttonDurations[durationIndex]) {
                proxyBridge->set_button_status(micras::proxy::Button::Status::NO_PRESS);
                buttonTimerActive = false;
            }
        }

        // Add interactive buttons for each status with duration
        ImGui::Separator();
        ImGui::Text("Set Button Status:");
        
        // Create a grid of buttons
        float buttonWidth = (ImGui::GetContentRegionAvail().x - 20) / 3; // 3 buttons per row with spacing
        float buttonHeight = 30;
        
        // First row
        if (ImGui::Button("Short Press", ImVec2(buttonWidth, buttonHeight))) {
            proxyBridge->set_button_status(micras::proxy::Button::Status::SHORT_PRESS);
            buttonActivationTime = std::chrono::steady_clock::now();
            buttonTimerActive = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Long Press", ImVec2(buttonWidth, buttonHeight))) {
            proxyBridge->set_button_status(micras::proxy::Button::Status::LONG_PRESS);
            buttonActivationTime = std::chrono::steady_clock::now();
            buttonTimerActive = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Extra Long Press", ImVec2(buttonWidth, buttonHeight))) {
            proxyBridge->set_button_status(micras::proxy::Button::Status::EXTRA_LONG_PRESS);
            buttonActivationTime = std::chrono::steady_clock::now();
            buttonTimerActive = true;
        }
              
        // Time remaining if a button is active
        if (buttonTimerActive && status != micras::proxy::Button::Status::NO_PRESS) {
            ImGui::Separator();
            auto currentTime = std::chrono::steady_clock::now();
            float elapsedTime = std::chrono::duration<float>(currentTime - buttonActivationTime).count();
            
            int durationIndex = 0;
            switch (status) {
                case micras::proxy::Button::Status::SHORT_PRESS: durationIndex = 0; break;
                case micras::proxy::Button::Status::LONG_PRESS: durationIndex = 1; break;
                case micras::proxy::Button::Status::EXTRA_LONG_PRESS: durationIndex = 2; break;
                default: break;
            }
            
            float remainingTime = buttonDurations[durationIndex] - elapsedTime;
            if (remainingTime < 0) remainingTime = 0;
            
            ImGui::Text("Time remaining: %.1f seconds", remainingTime);
            
            // Progress bar
            float progress = 1.0f - (remainingTime / buttonDurations[durationIndex]);
            ImGui::ProgressBar(progress, ImVec2(-1, 0), "");
        }
        
        // Debug information
        ImGui::Separator();
    }
    
    // Wall Sensors Section
    if (this->simulationEngine && ImGui::CollapsingHeader("Wall Sensors", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Columns(2, "wallsensors", false);
        for (size_t i = 0; i < 4; ++i) {
            ImGui::Text("Sensor %zu:", i);
            ImGui::NextColumn();
            ImGui::Text("%.4f", proxyBridge->get_wall_sensor_reading(i));
            ImGui::NextColumn();
        }
        ImGui::Columns(1);
    }

    // Simulation controls
    if (this->simulationEngine) {
        ImGui::SeparatorText("Maze Controls");

        if (this->renderEngine && this->renderEngine->mazeRender) {
            ImGui::Checkbox("Show Firmware Maze", &this->renderEngine->mazeRender->showFirmwareWalls);
        }
    
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
    
        
    }
    
    // Option to show style editor (defaults to off)
    ImGui::Checkbox("Show Style Editor", &showStyleEditor);
    ImGui::Checkbox("Show Performance Plots", &this->plot.showPlots);

    ImGui::End();

    // Only draw plots if not in fullscreen mode or if user explicitly enabled them
    if (this->plot.showPlots) {
        ImGui::Begin("Performance Plots");
        this->plot.draw(micrasBody, *proxyBridge, this->simulationEngine->isPaused);
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
