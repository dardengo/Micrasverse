#include "GUI/gui.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <filesystem>

namespace micrasverse::render {

GUI::GUI() {}

void GUI::setSimulationControl(const std::shared_ptr<micrasverse::simulation::SimulationControl>& simulationControl) {
    this->simulationControl = simulationControl;
}

void GUI::init(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
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

    ImGui::Begin("Micrasverse");
    ImGui::Text("Press 'R' to run simulation");
    ImGui::Text("Press 'F' to follow Micras");
    ImGui::Text("Press 'C' to center camera on maze");
    ImGui::Text("Press 'ESC' to exit");

    if (this->simulationControl) {
        ImGui::SeparatorText("Simulation Control");
    
        const auto& mazePaths = simulationControl->getMazePaths();
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
            /*
            if (ImGui::Button("Restart Maze")) {
                simulationControl->resetSimulation(mazePaths[selectedMazeIdx]);
            }*/
        } else {
            ImGui::TextColored(ImVec4(1,0.5f,0.5f,1), "No maze files found!");
        }
    
        // Toggle simulation running
        if (ImGui::Button(simulationControl->isRunning ? "Pause" : "Start")) {
            simulationControl->toggleSimulation();
        }
    
        ImGui::SameLine();
    
        // Step one frame
        if (ImGui::Button("Step")) {
            simulationControl->stepThroughSimulation();
        }
    
        ImGui::Text("Simulation is %s", simulationControl->isRunning ? "Running" : "Paused");
    }
    


    ImGui::Text("Micras position: (%.2f, %.2f)", micrasBody.getPosition().x, micrasBody.getPosition().y);
    ImGui::Text("Micras linear velocity: (%.2f, %.2f)", b2Body_GetLinearVelocity(micrasBody.getBodyId()).x, b2Body_GetLinearVelocity(micrasBody.getBodyId()).y);
    ImGui::Text("Micras angular velocity: %.2f", b2Body_GetAngularVelocity(micrasBody.getBodyId()));
    ImGui::Text("FAN is: %.2f", micrasBody.dipSwitch.readSwitch(micrasverse::physics::MicrasBody::Switch::FAN) ? 1.0f : 0.0f); ImGui::SameLine();
    ImGui::ColorEdit3("LED color", &micrasBody.argb.argbs[0].lightColorArray[0]);

    this->plot.draw(micrasBody);
    
    ImGui::End();

    ImGui::Begin("Style editor");
    ImGui::ShowStyleEditor();
    ImGui::End();
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
}

} // namespace micrasverse::render
