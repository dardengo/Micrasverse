#include "GUI/gui.hpp"

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

void GUI::draw(micrasverse::physics::MicrasBody& micrasBody) {
    // Get screen size directly from stored GLFW window
    int width, height;
    if (currentWindow) {
        glfwGetFramebufferSize(currentWindow, &width, &height);
        bool isLargeScreen = (width > 1600 || height > 900);
        
        // Optimize GUI for large screens
        if (isLargeScreen) {
            // Use smaller windows and simpler controls in fullscreen
            ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowSize(ImVec2(250, 350), ImGuiCond_FirstUseEver);
        } else {
            ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowSize(ImVec2(350, 450), ImGuiCond_FirstUseEver);
        }

        ImGui::Begin("Micrasverse");
        ImGui::Text("Press 'R' to run simulation");
        ImGui::Text("Press 'F' to follow Micras");
        ImGui::Text("Press 'C' to center camera on maze");
        ImGui::Text("Press 'ESC' to exit");
        ImGui::Text("Press 'F11' to toggle fullscreen");

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
        ImGui::Text("Micras linear velocity: (%.2f, %.2f)", b2Body_GetLinearVelocity(micrasBody.getBodyId()).x, b2Body_GetLinearVelocity(micrasBody.getBodyId()).y);
        ImGui::Text("Micras angular velocity: %.2f", b2Body_GetAngularVelocity(micrasBody.getBodyId()));
        ImGui::Text("FAN is: %.2f", micrasBody.dipSwitch.readSwitch(micrasverse::physics::MicrasBody::Switch::FAN) ? 1.0f : 0.0f); ImGui::SameLine();
        
        // Skip color editor in fullscreen mode to improve performance
        if (!isLargeScreen) {
            ImGui::ColorEdit3("LED color", &micrasBody.argb.argbs[0].lightColorArray[0]);
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
