#include "vulkan_engine/lve_imgui.hpp"

#include "vulkan_engine/lve_device.hpp"
#include "vulkan_engine/lve_window.hpp"

#include "constants.hpp"
#include "io/keyboard.hpp"

// libs
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

// std
#include <stdexcept>

namespace lve {

// ok this just initializes imgui using the provided integration files. So in our case we need to
// initialize the vulkan and glfw imgui implementations, since that's what our engine is built
// using.
LveImgui::LveImgui(LveWindow& window, LveDevice& device, VkRenderPass renderPass, uint32_t imageCount) : lveDevice{device} {
    // set up a descriptor pool stored on this instance, see header for more comments on this.
    VkDescriptorPoolSize pool_sizes[] = {
        {VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
        {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
        {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
        {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
        {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
        {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
        {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
        {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
        {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
        {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
        {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}
    };
    VkDescriptorPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
    pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
    pool_info.pPoolSizes = pool_sizes;
    if (vkCreateDescriptorPool(device.device(), &pool_info, nullptr, &descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to set up descriptor pool for imgui");
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    // Initialize imgui for vulkan
    ImGui_ImplGlfw_InitForVulkan(window.window, true);
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = device.getInstance();
    init_info.PhysicalDevice = device.getPhysicalDevice();
    init_info.Device = device.device();
    init_info.QueueFamily = device.getGraphicsQueueFamily();
    init_info.Queue = device.graphicsQueue();
    init_info.RenderPass = renderPass;

    // pipeline cache is a potential future optimization, ignoring for now
    init_info.PipelineCache = VK_NULL_HANDLE;
    init_info.DescriptorPool = descriptorPool;
    // todo, I should probably get around to integrating a memory allocator library such as Vulkan
    // memory allocator (VMA) sooner than later. We don't want to have to update adding an allocator
    // in a ton of locations.
    init_info.Allocator = VK_NULL_HANDLE;
    init_info.MinImageCount = 2;
    init_info.ImageCount = imageCount;
    init_info.CheckVkResultFn = check_vk_result;
    ImGui_ImplVulkan_Init(&init_info);

    // upload fonts, this is done by recording and submitting a one time use command buffer
    // which can be done easily bye using some existing helper functions on the lve device object
    auto commandBuffer = device.beginSingleTimeCommands();
    ImGui_ImplVulkan_CreateFontsTexture();
    device.endSingleTimeCommands(commandBuffer);

    this->plot.init();
}

LveImgui::~LveImgui() {
    vkDestroyDescriptorPool(lveDevice.device(), descriptorPool, nullptr);
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void LveImgui::newFrame() {
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void LveImgui::setSimulationEngine(const std::shared_ptr<micrasverse::simulation::SimulationEngine>& simulationEngine) {
    this->simulationEngine = simulationEngine;
}

void LveImgui::setProxyBridge(const std::shared_ptr<micras::ProxyBridge>& proxyBridge) {
    this->proxyBridge = proxyBridge;
}

// this tells imgui that we're done setting up the current frame,
// then gets the draw data from imgui and uses it to record to the provided
// command buffer the necessary draw commands
void LveImgui::render(VkCommandBuffer commandBuffer) {
    ImGui::Render();
    ImDrawData* drawdata = ImGui::GetDrawData();
    ImGui_ImplVulkan_RenderDrawData(drawdata, commandBuffer);
}

void LveImgui::runExample(micrasverse::physics::Box2DMicrasBody& micrasBody) {
    if (!proxyBridge) {
        ImGui::Begin("Error");
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Proxy Bridge not initialized!");
        ImGui::End();
        return;
    }

    // Main control panel
    ImGui::Begin("Micrasverse Control Panel");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

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
        ImGui::Text(
            "Micras Body Pose: (%.2f, %.2f, %.2f)", micrasBody.getPosition().x - micrasverse::WALL_THICKNESS,
            micrasBody.getPosition().y - micrasverse::WALL_THICKNESS, micrasBody.getAngle() + B2_PI / 2.0f
        );
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
            ImVec4      objectiveColor;

            switch (proxyBridge->get_objective()) {
                case micras::core::Objective::EXPLORE:
                    objectiveColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);  // Green
                    break;
                case micras::core::Objective::RETURN:
                    objectiveColor = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);  // Blue
                    break;
                case micras::core::Objective::SOLVE:
                    objectiveColor = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);  // Orange
                    break;
                default:
                    objectiveColor = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);  // Gray
                    break;
            }

            ImGui::PushStyleColor(ImGuiCol_Text, objectiveColor);
            ImGui::Text("%s", objectiveText.c_str());
            ImGui::PopStyleColor();

            // Display current action with color coding
            std::string actionText = "Current Action: " + proxyBridge->get_action_type_string();
            ImVec4      actionColor = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);  // Default gray

            // Use simpler approach since we don't have direct access to the Action type
            ImGui::PushStyleColor(ImGuiCol_Text, actionColor);
            ImGui::Text("%s", actionText.c_str());
            ImGui::PopStyleColor();

            // Set Objective buttons using Interface events
            ImGui::Separator();
            ImGui::Text("Set Objective via Interface Events:");

            if (ImGui::Button("Set EXPLORE")) {
                proxyBridge->send_event(micras::Interface::Event::EXPLORE);
            }

            ImGui::SameLine();

            if (ImGui::Button("Set SOLVE")) {
                proxyBridge->send_event(micras::Interface::Event::SOLVE);
            }

            ImGui::SameLine();

            if (ImGui::Button("CALIBRATE")) {
                proxyBridge->send_event(micras::Interface::Event::CALIBRATE);
            }
        }
    }

    // Movement Controls Section
    if (ImGui::CollapsingHeader("Movement Controls")) {
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
                if (wPressed)
                    linearSpeed = moveSpeed;
                if (sPressed)
                    linearSpeed = -moveSpeed;

                // Left/Right turning
                float angularSpeed = 0.0f;
                if (aPressed)
                    angularSpeed = moveSpeed;
                if (dPressed)
                    angularSpeed = -moveSpeed;

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
    if (ImGui::CollapsingHeader("DIP Switches")) {
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

    // Add LED/ARGB Control Section
    if (ImGui::CollapsingHeader("LED Control")) {
        ImGui::Text("Set LED Colors by Events:");

        // Create grid of buttons for different events and colors
        float buttonWidth = (ImGui::GetContentRegionAvail().x - 20) / 2;
        float buttonHeight = 30;

        // ARGB Control for events
        if (ImGui::Button("Set EXPLORE Color (Green)", ImVec2(buttonWidth, buttonHeight))) {
            micrasverse::types::Color color{0, 255, 0};  // Green
            proxyBridge->set_argb_color(color);
            proxyBridge->send_event(micras::Interface::Event::EXPLORE);
        }

        ImGui::SameLine();

        if (ImGui::Button("Set SOLVE Color (Blue)", ImVec2(buttonWidth, buttonHeight))) {
            micrasverse::types::Color color{0, 0, 255};  // Blue
            proxyBridge->set_argb_color(color);
            proxyBridge->send_event(micras::Interface::Event::SOLVE);
        }

        if (ImGui::Button("Set CALIBRATE Color (Yellow)", ImVec2(buttonWidth, buttonHeight))) {
            micrasverse::types::Color color{255, 255, 0};  // Yellow
            proxyBridge->set_argb_color(color);
            proxyBridge->send_event(micras::Interface::Event::CALIBRATE);
        }

        ImGui::SameLine();

        if (ImGui::Button("Set ERROR Color (Red)", ImVec2(buttonWidth, buttonHeight))) {
            micrasverse::types::Color color{255, 0, 0};  // Red
            proxyBridge->set_argb_color(color);
            proxyBridge->send_event(micras::Interface::Event::ERROR);
        }

        if (ImGui::Button("Turn Off LEDs", ImVec2(buttonWidth, buttonHeight))) {
            proxyBridge->turn_off_argb();
        }

        // Manual control of individual LEDs
        ImGui::Separator();
        ImGui::Text("Manual Control:");

        static int   led_index = 0;
        static float color[3] = {255.0f, 255.0f, 255.0f};  // RGB

        ImGui::SliderInt("LED Index", &led_index, 0, 1);
        ImGui::ColorEdit3("LED Color", color);

        if (ImGui::Button("Set Individual LED")) {
            proxyBridge->set_led_color(led_index, color[0], color[1], color[2]);
        }
    }

    // Button Controls Section
    if (ImGui::CollapsingHeader("Button Controls")) {
        // Get the current button status
        auto        status = proxyBridge->get_button_status();
        std::string statusText;
        ImVec4      statusColor;

        // Check if a button timer is active and handle timeout
        if (buttonTimerActive) {
            auto  currentTime = std::chrono::steady_clock::now();
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
            if (status != micras::proxy::Button::Status::NO_PRESS && elapsedTime >= buttonDurations[durationIndex]) {
                proxyBridge->set_button_status(micras::proxy::Button::Status::NO_PRESS);
                buttonTimerActive = false;
            }
        }

        // Add interactive buttons for each status with duration
        ImGui::Separator();
        ImGui::Text("Set Button Status:");

        // Create a grid of buttons
        float buttonWidth = (ImGui::GetContentRegionAvail().x - 20) / 3;  // 3 buttons per row with spacing
        float buttonHeight = 30;

        // Use Interface events instead of direct button status setting
        if (ImGui::Button("Short Press (EXPLORE)", ImVec2(buttonWidth, buttonHeight))) {
            proxyBridge->set_button_status(micras::proxy::Button::Status::SHORT_PRESS);
            buttonActivationTime = std::chrono::steady_clock::now();
            buttonTimerActive = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Long Press (SOLVE)", ImVec2(buttonWidth, buttonHeight))) {
            proxyBridge->set_button_status(micras::proxy::Button::Status::LONG_PRESS);
            buttonActivationTime = std::chrono::steady_clock::now();
            buttonTimerActive = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Extra Long Press (CALIBRATE)", ImVec2(buttonWidth, buttonHeight))) {
            proxyBridge->set_button_status(micras::proxy::Button::Status::EXTRA_LONG_PRESS);
            buttonActivationTime = std::chrono::steady_clock::now();
            buttonTimerActive = true;
        }

        // Time remaining if a button is active
        if (buttonTimerActive && status != micras::proxy::Button::Status::NO_PRESS) {
            ImGui::Separator();
            auto  currentTime = std::chrono::steady_clock::now();
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
                    break;
            }

            float remainingTime = buttonDurations[durationIndex] - elapsedTime;
            if (remainingTime < 0)
                remainingTime = 0;

            ImGui::Text("Time remaining: %.1f seconds", remainingTime);

            // Progress bar
            float progress = 1.0f - (remainingTime / buttonDurations[durationIndex]);
            ImGui::ProgressBar(progress, ImVec2(-1, 0), "");
        }

        // Show Interface events status
        ImGui::Separator();
        ImGui::Text("Interface Events:");

        bool exploreEvent = proxyBridge->peek_event(micras::Interface::Event::EXPLORE);
        bool solveEvent = proxyBridge->peek_event(micras::Interface::Event::SOLVE);
        bool calibrateEvent = proxyBridge->peek_event(micras::Interface::Event::CALIBRATE);
        bool errorEvent = proxyBridge->peek_event(micras::Interface::Event::ERROR);

        ImGui::Text("EXPLORE: %s", exploreEvent ? "Active" : "Inactive");
        ImGui::Text("SOLVE: %s", solveEvent ? "Active" : "Inactive");
        ImGui::Text("CALIBRATE: %s", calibrateEvent ? "Active" : "Inactive");
        ImGui::Text("ERROR: %s", errorEvent ? "Active" : "Inactive");

        // Allow acknowledging events
        if (ImGui::Button("Acknowledge All Events")) {
            proxyBridge->acknowledge_event(micras::Interface::Event::EXPLORE);
            proxyBridge->acknowledge_event(micras::Interface::Event::SOLVE);
            proxyBridge->acknowledge_event(micras::Interface::Event::CALIBRATE);
            proxyBridge->acknowledge_event(micras::Interface::Event::ERROR);
        }
    }

    // Add Buzzer Control Section
    if (ImGui::CollapsingHeader("Buzzer Control")) {
        static float frequency = 440.0f;  // Default to 440 Hz (A4 note)
        static int   duration = 500;      // Default to 500 ms

        ImGui::SliderFloat("Frequency (Hz)", &frequency, 20.0f, 20000.0f, "%.1f", ImGuiSliderFlags_Logarithmic);
        ImGui::SliderInt("Duration (ms)", &duration, 100, 5000);

        if (ImGui::Button("Play Sound")) {
            proxyBridge->set_buzzer_frequency(frequency);
            proxyBridge->set_buzzer_duration(duration);
        }

        ImGui::SameLine();

        if (ImGui::Button("Stop Sound")) {
            proxyBridge->stop_buzzer();
        }

        // Display current buzzer status
        bool isPlaying = proxyBridge->is_buzzer_playing();
        ImGui::Text("Buzzer is %s", isPlaying ? "playing" : "stopped");

        // Predefined tones/patterns
        ImGui::Separator();
        ImGui::Text("Predefined Patterns:");

        if (ImGui::Button("Error Tone")) {
            // Play error tone and trigger error event
            proxyBridge->set_buzzer_frequency(880.0f);  // Higher frequency for error
            proxyBridge->set_buzzer_duration(300);
            proxyBridge->send_event(micras::Interface::Event::ERROR);
        }

        ImGui::SameLine();

        if (ImGui::Button("Success Tone")) {
            // Play success tone and trigger explore event
            proxyBridge->set_buzzer_frequency(1760.0f);
            proxyBridge->set_buzzer_duration(200);
            proxyBridge->send_event(micras::Interface::Event::EXPLORE);
        }
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

        const auto& mazePaths = simulationEngine->getMazePaths();
        static int  selectedMazeIdx = 0;

        if (!mazePaths.empty()) {
            // Only use filenames for the dropdown
            std::string currentMazeFile = std::filesystem::path(mazePaths[selectedMazeIdx]).filename().string();

            if (ImGui::BeginCombo("Maze", currentMazeFile.c_str())) {
                for (int i = 0; i < mazePaths.size(); ++i) {
                    std::string label = std::filesystem::path(mazePaths[i]).filename().string();
                    bool        selected = (selectedMazeIdx == i);
                    if (ImGui::Selectable(label.c_str(), selected)) {
                        selectedMazeIdx = i;
                    }
                    if (selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            // Restart simulation with selected maze

            if (ImGui::Button("Load Maze")) {
                simulationEngine->resetSimulation(mazePaths[selectedMazeIdx]);
            }

            if (ImGui::Button("Reset Simulation")) {
                simulationEngine->resetSimulation();
                proxyBridge->reset_micras();
            }

        } else {
            ImGui::TextColored(ImVec4(1, 0.5f, 0.5f, 1), "No maze files found!");
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

}  // namespace lve
