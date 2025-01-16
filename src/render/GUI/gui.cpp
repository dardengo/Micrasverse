#include "GUI/gui.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace micrasverse::render {

GUI::GUI() {}

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
    ImGui::Text("Micras position: (%.2f, %.2f)", micrasBody.getPosition().x, micrasBody.getPosition().y);
    ImGui::Text("Micras linear velocity: (%.2f, %.2f)", b2Body_GetLinearVelocity(micrasBody.getBodyId()).x, b2Body_GetLinearVelocity(micrasBody.getBodyId()).y);
    ImGui::Text("Micras angular velocity: %.2f", b2Body_GetAngularVelocity(micrasBody.getBodyId()));
    ImGui::Text("FAN is: %.2f", micrasBody.dipSwitch.readSwitch(micrasverse::physics::MicrasBody::Switch::FAN) ? 1.0f : 0.0f);

    this->plot.draw(micrasBody);
    
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
