#include "GUI/plot.hpp"
#include "physics/box2d_motor.hpp"
#include "physics/box2d_distance_sensor.hpp"
#include "implot.h"
#include "implot3d.h"

#include <cmath>
#include <algorithm>
#include <random>
#include <cfloat>

namespace micrasverse::render {

// Random color generator for plot variables
static ImVec4 RandomColor() {
    static std::mt19937                          rng(std::random_device{}());
    static std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    return ImVec4(dist(rng), dist(rng), dist(rng), 1.0f);
}

Plot::PlotVariable::PlotVariable(const std::string& name, const std::string& label, const ImVec4& color) :
    name(name), label(label), color(color), selected(false) {
    data.reserve(2000);
}

Plot::ScrollingBuffer::ScrollingBuffer(int maxSize) {
    this->maxSize = maxSize;
    this->offset = 0;
}

void Plot::ScrollingBuffer::addPoint(const float x, const float y) {
    if (this->data.size() < this->maxSize)
        this->data.push_back(ImVec2(x, y));
    else {
        this->data[this->offset] = ImVec2(x, y);
        this->offset = (this->offset + 1) % this->maxSize;
    }
}

void Plot::ScrollingBuffer::erase() {
    if (this->data.size() > 0) {
        this->data.shrink(0);
        this->offset = 0;
    }
}

Plot::RollingBuffer::RollingBuffer(int span, int maxSize) {
    this->span = span;
}

void Plot::RollingBuffer::addPoint(float x, float y) {
    float xmod = std::fmod(x, this->span);

    if (!this->data.empty() && xmod < this->data.back().x) {
        this->data.shrink(0);
    }

    this->data.push_back(ImVec2(xmod, y));
}

Plot::Plot() {
    showPlots = true;
    showDragAndDropMode = false;
    showMazeCostVisualizations = false;
    updateInterval = 0.1f;
    timeSinceLastUpdate = 0.0f;
    t = 0.0f;
    variablesInitialized = false;
    mazeCostVisualizationInitialized = false;
    mazeCostColorScale = 1.0f;
    history = 30.0f;

    // Initialize drag rectangle - will be updated in drawDragAndDrop
    // We'll use a default width of 1/4 of the history
    dragRect = ImPlotRect(0.0f, history / 4.0f, -1.0f, 1.0f);
    dragRectClicked = false;
    dragRectHovered = false;
    dragRectHeld = false;
}

void Plot::init() {
    ImPlot::CreateContext();
    ImPlot3D::CreateContext();
}

void Plot::drawDragAndDrop(micrasverse::physics::Box2DMicrasBody& micrasBody, micras::ProxyBridge& proxyBridge, bool simulationIsPaused) {
    if (!showPlots || !showDragAndDropMode) {
        return;
    }

    // Initialize and update plot variables
    initPlotVariables(micrasBody, proxyBridge);

    if (!simulationIsPaused) {
        updatePlotVariables(micrasBody, proxyBridge);

        // Add configurable settings for the DragRect behavior
        static bool autoRepositionRect = true;
        ImGui::SameLine();
        ImGui::Checkbox("Auto-reposition rect when out of view", &autoRepositionRect);

        // Only reposition if the rect is completely out of view
        // and not being manipulated by the user
        if (autoRepositionRect && !dragRectHeld && !dragRectHovered && (dragRect.X.Max < t - history || dragRect.X.Min > t)) {
            float visibleRange = history;
            float rectWidth = visibleRange * 0.25f;  // 25% of visible range

            // Position at the right side of the visible range
            dragRect.X.Min = t - rectWidth - 0.1f * visibleRange;  // little offset from right edge
            dragRect.X.Max = t - 0.1f * visibleRange;
        }
    }

    // Control for plot history
    ImGui::SliderFloat("History", &history, 1, 30, "%.1f s");

    // Add more control for the DragRect
    static ImPlotDragToolFlags dragFlags = ImPlotDragToolFlags_None;
    ImGui::SameLine();
    if (ImGui::Button("Reset Drag Rect")) {
        // Reset to a good position on demand
        float visibleRange = history;
        float rectWidth = visibleRange * 0.25f;
        dragRect.X.Min = t - rectWidth - 0.1f * visibleRange;
        dragRect.X.Max = t - 0.1f * visibleRange;

        // Find a reasonable Y range from the data in chart 4
        float minY = FLT_MAX;
        float maxY = -FLT_MAX;
        for (int varIdx : selectedVariablesForCharts[4]) {
            if (varIdx >= 0 && varIdx < plotVariables.size() && !plotVariables[varIdx].data.empty()) {
                for (const auto& point : plotVariables[varIdx].data) {
                    if (point.x >= t - history && point.x <= t) {
                        minY = std::min(minY, point.y);
                        maxY = std::max(maxY, point.y);
                    }
                }
            }
        }

        if (minY != FLT_MAX && maxY != -FLT_MAX) {
            float range = maxY - minY;
            dragRect.Y.Min = minY - 0.1f * range;
            dragRect.Y.Max = maxY + 0.1f * range;
        } else {
            dragRect.Y.Min = -1.0f;
            dragRect.Y.Max = 1.0f;
        }
    }
    // Variables pool section
    ImGui::BeginChild("Variables", ImVec2(200, 600), true);
    ImGui::Text("Variables Pool");
    ImGui::Separator();

    for (int i = 0; i < plotVariables.size(); i++) {
        auto& var = plotVariables[i];
        ImPlot::ItemIcon(var.color);
        ImGui::SameLine();
        if (ImGui::Selectable(var.label.c_str(), var.selected)) {
            var.selected = !var.selected;
        }

        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
            ImGui::SetDragDropPayload("PLOT_VARIABLE", &i, sizeof(int));
            ImPlot::ItemIcon(var.color);
            ImGui::SameLine();
            ImGui::TextUnformatted(var.label.c_str());
            ImGui::EndDragDropSource();
        }
    }
    ImGui::EndChild();

    ImGui::SameLine();

    // Charts section
    ImGui::BeginChild("Charts", ImVec2(-1, 600), true);

    static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;

    // Draw 5 line charts
    for (int chartIndex = 0; chartIndex < 5; chartIndex++) {
        char plotLabel[32];
        snprintf(plotLabel, sizeof(plotLabel), "##Chart%d", chartIndex);

        if (ImPlot::BeginPlot(plotLabel, ImVec2(-1, 80), ImPlotFlags_NoFrame | ImPlotFlags_NoTitle)) {
            ImPlot::SetupAxes(nullptr, nullptr, flags, flags);
            ImPlot::SetupAxisLimits(ImAxis_X1, t - history, t, ImGuiCond_Always);

            // Auto-adjust Y axis limits based on data
            float minY = FLT_MAX;
            float maxY = -FLT_MAX;
            for (int varIdx : selectedVariablesForCharts[chartIndex]) {
                if (varIdx >= 0 && varIdx < plotVariables.size() && !plotVariables[varIdx].data.empty()) {
                    for (const auto& point : plotVariables[varIdx].data) {
                        if (point.x >= t - history) {
                            minY = std::min(minY, point.y);
                            maxY = std::max(maxY, point.y);
                        }
                    }
                }
            }

            // Add some margin
            if (minY != FLT_MAX && maxY != -FLT_MAX) {
                float range = maxY - minY;
                ImPlot::SetupAxisLimits(ImAxis_Y1, minY - 0.1f * range, maxY + 0.1f * range);
            } else {
                ImPlot::SetupAxisLimits(ImAxis_Y1, -1, 1);
            }

            // Plot selected variables
            for (int varIdx : selectedVariablesForCharts[chartIndex]) {
                if (varIdx >= 0 && varIdx < plotVariables.size() && !plotVariables[varIdx].data.empty()) {
                    auto& var = plotVariables[varIdx];
                    ImPlot::SetNextLineStyle(var.color);
                    ImPlot::PlotLine(var.label.c_str(), &var.data[0].x, &var.data[0].y, var.data.size(), 0, 0, 2 * sizeof(float));
                }
            }

            // Make the last chart have a DragRect
            if (chartIndex == 4) {
                // Only adjust the Y range on initialization or when the data changes drastically
                if (!dragRectHeld && !dragRectHovered && dragRectClicked == false) {
                    // Initialize the Y range only if it's way out of bounds or unset
                    float minDataY = minY;
                    float maxDataY = maxY;

                    if (minDataY != FLT_MAX && maxDataY != -FLT_MAX) {
                        float dataRange = maxDataY - minDataY;

                        // Only reset Y range if current range is invalid or completely outside data bounds
                        if (dragRect.Y.Min >= dragRect.Y.Max || (dragRect.Y.Max < minDataY - dataRange) || (dragRect.Y.Min > maxDataY + dataRange)) {
                            // Set to full data range with margin
                            dragRect.Y.Min = minDataY - 0.1f * dataRange;
                            dragRect.Y.Max = maxDataY + 0.1f * dataRange;
                        }
                    }
                }

                // Make the drag rect fully interactive
                ImPlot::DragRect(
                    0, &dragRect.X.Min, &dragRect.Y.Min, &dragRect.X.Max, &dragRect.Y.Max, ImVec4(1, 0, 1, 0.5), dragFlags, &dragRectClicked,
                    &dragRectHovered, &dragRectHeld
                );
            }

            // Enable drag and drop for charts
            if (ImPlot::BeginDragDropTargetPlot()) {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PLOT_VARIABLE")) {
                    int varIdx = *(int*)payload->Data;
                    // Add variable if not already in chart
                    if (std::find(selectedVariablesForCharts[chartIndex].begin(), selectedVariablesForCharts[chartIndex].end(), varIdx) ==
                        selectedVariablesForCharts[chartIndex].end()) {
                        selectedVariablesForCharts[chartIndex].push_back(varIdx);
                    }
                }
                ImPlot::EndDragDropTarget();
            }

            ImPlot::EndPlot();
        }

        // Right-click context menu for chart
        if (ImGui::BeginPopupContextItem()) {
            ImGui::Text("Chart %d Variables", chartIndex + 1);
            ImGui::Separator();

            for (auto it = selectedVariablesForCharts[chartIndex].begin(); it != selectedVariablesForCharts[chartIndex].end();) {
                int varIdx = *it;
                if (varIdx >= 0 && varIdx < plotVariables.size()) {
                    auto& var = plotVariables[varIdx];
                    ImPlot::ItemIcon(var.color);
                    ImGui::SameLine();
                    if (ImGui::Selectable(("Remove " + var.label).c_str())) {
                        it = selectedVariablesForCharts[chartIndex].erase(it);
                    } else {
                        ++it;
                    }
                } else {
                    it = selectedVariablesForCharts[chartIndex].erase(it);
                }
            }

            ImGui::EndPopup();
        }
    }

    // Add the 6th chart for the magnified view (detail chart)
    ImVec4 bg_col =
        dragRectHeld ? ImVec4(0.5f, 0, 0.5f, 1) : (dragRectHovered ? ImVec4(0.25f, 0, 0.25f, 1) : ImPlot::GetStyle().Colors[ImPlotCol_PlotBg]);
    ImPlot::PushStyleColor(ImPlotCol_PlotBg, bg_col);

    // Add details about the selection first
    char  details[256];
    float timeSpan = dragRect.X.Max - dragRect.X.Min;
    float valueSpan = dragRect.Y.Max - dragRect.Y.Min;
    snprintf(
        details, sizeof(details), "Selection: t=[%.2f to %.2f] (%.2f s), y=[%.2f to %.2f] (range: %.2f)", dragRect.X.Min, dragRect.X.Max, timeSpan,
        dragRect.Y.Min, dragRect.Y.Max, valueSpan
    );
    ImGui::TextColored(ImVec4(1, 1, 1, 1), "%s", details);

    if (ImPlot::BeginPlot("##DetailView", ImVec2(-1, 100), ImPlotFlags_NoFrame | ImPlotFlags_NoTitle)) {
        ImPlot::SetupAxes(nullptr, nullptr, flags, flags);
        ImPlot::SetupAxisLimits(ImAxis_X1, dragRect.X.Min, dragRect.X.Max, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1, dragRect.Y.Min, dragRect.Y.Max, ImGuiCond_Always);

        // Plot the selected variables for the detail view (same as the last chart)
        for (int varIdx : selectedVariablesForCharts[4]) {
            if (varIdx >= 0 && varIdx < plotVariables.size() && !plotVariables[varIdx].data.empty()) {
                auto& var = plotVariables[varIdx];
                ImPlot::SetNextLineStyle(var.color);
                ImPlot::PlotLine(var.label.c_str(), &var.data[0].x, &var.data[0].y, var.data.size(), 0, 0, 2 * sizeof(float));
            }
        }

        ImPlot::EndPlot();
    }

    ImPlot::PopStyleColor();

    ImGui::Text(
        "Drag rectangle: %sclicked, %shovered, %sheld", dragRectClicked ? "" : "not ", dragRectHovered ? "" : "not ", dragRectHeld ? "" : "not "
    );

    ImGui::EndChild();
}

void Plot::draw(micrasverse::physics::Box2DMicrasBody& micrasBody, micras::ProxyBridge& proxyBridge, bool simulationIsPaused) {
    if (!showPlots) {
        return;
    }

    // Toggle between different modes
    ImGui::Checkbox("Drag and Drop Mode", &showDragAndDropMode);
    ImGui::SameLine();
    ImGui::Checkbox("Maze Cost Visualizations", &showMazeCostVisualizations);

    // Slider to select the time history to display
    ImGui::SliderFloat("Time History", &history, 0.1f, 60.0f, "%.1f");

    // If Maze Cost mode is active, draw those charts
    if (showMazeCostVisualizations) {
        ImGui::BeginChild("MazeCostVisualizations", ImVec2(-1, -1), false);

        ImGui::SliderFloat("Color Scale", &mazeCostColorScale, 0.1f, 5.0f, "%.1f");
        ImGui::Columns(2);  // Split into 2 columns for side-by-side charts

        drawMazeCostHeatmap(proxyBridge);
        ImGui::NextColumn();
        drawMazeCost3DSurface(proxyBridge);

        ImGui::Columns(1);

        std::vector<Node> nodes = {{1, 1.f, 1.f, micras::nav::RIGHT}, {2, 1.f, 2.f, micras::nav::UP}, {3, 2.f, 2.f, micras::nav::LEFT}};

        std::vector<Edge> edges = {{1, 2}, {2, 3}, {3, 1}};

        drawGraph(proxyBridge.get_maze_graph());

        ImGui::EndChild();
        return;
    }

    // If DnD mode is active, use that drawing method instead
    if (showDragAndDropMode) {
        drawDragAndDrop(micrasBody, proxyBridge, simulationIsPaused);
        return;
    }

    static ScrollingBuffer sdata1, sdata2, sdata3, sdata4, sdata5, sdata6, sdata7, sdata8, sdata9, sdata10;
    static ScrollingBuffer rdata1, rdata2, rdata3, rdata4, rdata5, rdata6, rdata7, rdata8, rdata9, rdata10, rdata11, rdata12, rdata13, rdata14,
        rdata15, rdata16, rdata17, rdata18, rdata19, rdata20, rdata21, rdata22, rdata23, rdata24, rdata25, rdata26, rdata27, rdata28, rdata29,
        rdata30;

    // Ensure buffers are not empty at first launch
    static bool initialized = false;
    if (!initialized) {
        // Add initial data points with zero values
        const float initialTime = 0.0f;
        for (float t = 0; t < 0.1f; t += 0.1f) {
            sdata1.addPoint(t, 0.0f);
            sdata2.addPoint(t, 0.0f);
            sdata3.addPoint(t, 0.0f);
            sdata4.addPoint(t, 0.0f);
            sdata5.addPoint(t, 0.0f);
            sdata6.addPoint(t, 0.0f);
            sdata7.addPoint(t, 0.0f);
            sdata8.addPoint(t, 0.0f);
            sdata9.addPoint(t, 0.0f);
            sdata10.addPoint(t, 0.0f);

            rdata1.addPoint(t, 0.0f);
            rdata2.addPoint(t, 0.0f);
            rdata3.addPoint(t, 0.0f);
            rdata4.addPoint(t, 0.0f);
            rdata5.addPoint(t, 0.0f);
            rdata6.addPoint(t, 0.0f);
            rdata7.addPoint(t, 0.0f);
            rdata8.addPoint(t, 0.0f);
            rdata9.addPoint(t, 0.0f);
            rdata10.addPoint(t, 0.0f);
            rdata11.addPoint(t, 0.0f);
            rdata12.addPoint(t, 0.0f);
            rdata13.addPoint(t, 0.0f);
            rdata14.addPoint(t, 0.0f);
            rdata15.addPoint(t, 0.0f);
            rdata16.addPoint(t, 0.0f);
            rdata17.addPoint(t, 0.0f);
            rdata18.addPoint(t, 0.0f);
            rdata19.addPoint(t, 0.0f);
            rdata20.addPoint(t, 0.0f);
            rdata21.addPoint(t, 0.0f);
            rdata22.addPoint(t, 0.0f);
            rdata23.addPoint(t, 0.0f);
            rdata24.addPoint(t, 0.0f);
            rdata25.addPoint(t, 0.0f);
            rdata26.addPoint(t, 0.0f);
            rdata27.addPoint(t, 0.0f);
        }
        initialized = true;
    }

    static float t = 0;

    // Only add data points if simulation is not paused
    if (!simulationIsPaused) {
        float deltaTime = ImGui::GetIO().DeltaTime;
        t += deltaTime;
        // Add data points
        sdata1.addPoint(t, micrasBody.getRightMotor().getCurrent());
        sdata2.addPoint(t, micrasBody.getLeftMotor().getCurrent());

        rdata1.addPoint(t, micrasBody.getRightMotor().getAngularVelocity());
        rdata2.addPoint(t, micrasBody.getLeftMotor().getAngularVelocity());

        rdata3.addPoint(t, micrasBody.getRightMotor().getBodyAngularVelocity());
        rdata4.addPoint(t, micrasBody.getLeftMotor().getBodyLinearVelocity());
        rdata5.addPoint(t, micrasBody.getLinearAcceleration());
        rdata6.addPoint(t, proxyBridge.get_linear_speed());
        rdata19.addPoint(t, proxyBridge.get_angular_speed());
        rdata20.addPoint(t, micrasBody.getLinearSpeed());

        rdata7.addPoint(t, proxyBridge.get_wall_sensor_reading(0));
        rdata8.addPoint(t, proxyBridge.get_wall_sensor_reading(1));
        rdata9.addPoint(t, -proxyBridge.get_wall_sensor_reading(2));
        rdata10.addPoint(t, proxyBridge.get_wall_sensor_reading(3));

        sdata7.addPoint(t, proxyBridge.get_wall_sensor_reading(0));
        sdata8.addPoint(t, proxyBridge.get_wall_sensor_reading(1));
        sdata9.addPoint(t, -proxyBridge.get_wall_sensor_reading(2));
        sdata10.addPoint(t, proxyBridge.get_wall_sensor_reading(3));

        rdata11.addPoint(t, micrasBody.getRightMotor().getAppliedForce());
        rdata12.addPoint(t, micrasBody.getLeftMotor().getAppliedForce());

        // PID controller data
        rdata13.addPoint(t, proxyBridge.get_linear_pid_setpoint());
        rdata14.addPoint(t, proxyBridge.get_angular_pid_setpoint());
        rdata15.addPoint(t, proxyBridge.get_linear_pid_last_response());
        rdata16.addPoint(t, proxyBridge.get_angular_pid_last_response());

        rdata17.addPoint(t, proxyBridge.get_left_feed_forward_response());
        rdata18.addPoint(t, proxyBridge.get_right_feed_forward_response());

        rdata21.addPoint(t, proxyBridge.get_linear_integrative_response());
        rdata22.addPoint(t, proxyBridge.get_angular_integrative_response());
    }

    static ImPlotAxisFlags flags = ImPlotAxisFlags_NoInitialFit | ImPlotAxisFlags_AutoFit;

    if (!sdata1.data.empty() && ImPlot::BeginPlot("Linear PID setpoint", ImVec2(-1, 150), ImPlotFlags_NoFrame | ImPlotFlags_NoTitle)) {
        ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoTickLabels, flags);
        ImPlot::SetupAxisLimits(ImAxis_X1, t - history, t, ImGuiCond_Always);
        ImPlot::PlotLine("Linear PID setpoint", &rdata13.data[0].x, &rdata13.data[0].y, rdata13.data.size(), 0, rdata13.offset, 2 * sizeof(float));
        ImPlot::PlotLine("Linear speed", &rdata6.data[0].x, &rdata6.data[0].y, rdata6.data.size(), 0, rdata6.offset, 2 * sizeof(float));
        ImPlot::PlotLine("Body linear speed", &rdata20.data[0].x, &rdata20.data[0].y, rdata20.data.size(), 0, rdata20.offset, 2 * sizeof(float));
        ImPlot::EndPlot();
    }

    if (!sdata1.data.empty() && ImPlot::BeginPlot("Angular PID setpoint", ImVec2(-1, 150), ImPlotFlags_NoFrame | ImPlotFlags_NoTitle)) {
        ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoTickLabels, flags);
        ImPlot::SetupAxisLimits(ImAxis_X1, t - history, t, ImGuiCond_Always);
        ImPlot::PlotLine("Angular PID setpoint", &rdata14.data[0].x, &rdata14.data[0].y, rdata14.data.size(), 0, rdata14.offset, 2 * sizeof(float));
        ImPlot::PlotLine("Angular speed", &rdata19.data[0].x, &rdata19.data[0].y, rdata19.data.size(), 0, rdata19.offset, 2 * sizeof(float));
        ImPlot::EndPlot();
    }

    if (!sdata1.data.empty() && ImPlot::BeginPlot("Linear PID response", ImVec2(-1, 150), ImPlotFlags_NoFrame | ImPlotFlags_NoTitle)) {
        ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoTickLabels, flags);
        ImPlot::SetupAxisLimits(ImAxis_X1, t - history, t, ImGuiCond_Always);
        ImPlot::PlotLine("Linear PID response", &rdata15.data[0].x, &rdata15.data[0].y, rdata15.data.size(), 0, rdata15.offset, 2 * sizeof(float));
        ImPlot::PlotLine(
            "Linear integrative response", &rdata21.data[0].x, &rdata21.data[0].y, rdata21.data.size(), 0, rdata21.offset, 2 * sizeof(float)
        );
        ImPlot::EndPlot();
    }

    if (!sdata1.data.empty() && ImPlot::BeginPlot("Angular PID response", ImVec2(-1, 150), ImPlotFlags_NoFrame | ImPlotFlags_NoTitle)) {
        ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoTickLabels, flags);
        ImPlot::SetupAxisLimits(ImAxis_X1, t - history, t, ImGuiCond_Always);
        ImPlot::PlotLine("Angular PID response", &rdata16.data[0].x, &rdata16.data[0].y, rdata16.data.size(), 0, rdata16.offset, 2 * sizeof(float));
        ImPlot::PlotLine(
            "Angular integrative response", &rdata22.data[0].x, &rdata22.data[0].y, rdata22.data.size(), 0, rdata22.offset, 2 * sizeof(float)
        );
        ImPlot::EndPlot();
    }

    if (!sdata1.data.empty() && ImPlot::BeginPlot("Feed forward response", ImVec2(-1, 150), ImPlotFlags_NoFrame | ImPlotFlags_NoTitle)) {
        ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoTickLabels, flags);
        ImPlot::SetupAxisLimits(ImAxis_X1, t - history, t, ImGuiCond_Always);
        ImPlot::PlotLine(
            "Left feed forward response", &rdata17.data[0].x, &rdata17.data[0].y, rdata17.data.size(), 0, rdata17.offset, 2 * sizeof(float)
        );
        ImPlot::PlotLine(
            "Right feed forward response", &rdata18.data[0].x, &rdata18.data[0].y, rdata18.data.size(), 0, rdata18.offset, 2 * sizeof(float)
        );
        ImPlot::EndPlot();
    }
}

void Plot::drawGraph(std::unordered_map<micras::nav::GridPose, micras::nav::MazeGraph::Node> graph) {
    ImPlot::PushStyleVar(ImPlotStyleVar_MinorGridSize, ImVec2(3.0f, 3.0f));

    if (ImPlot::BeginPlot("Maze Graph", ImVec2(-1, 800), ImPlotFlags_NoFrame | ImPlotFlags_NoTitle | ImPlotFlags_NoLegend)) {
        ImPlot::SetupAxes("X", "Y", ImPlotAxisFlags_NoTickLabels, ImPlotAxisFlags_NoTickLabels);
        ImPlot::SetupAxisLimits(ImAxis_X1, 0.0, 16, ImPlotCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1, 0.0, 16, ImPlotCond_Always);

        ImPlot::SetupAxisTicks(ImAxis_X1, 0, 16, 17);
        ImPlot::SetupAxisTicks(ImAxis_Y1, 0, 16, 17);

        // Create a map for quick node lookup
        std::unordered_map<micras::nav::GridPose, ImVec2> nodeMap;

        // Collect all nodes and create lookup map
        for (const auto& [pose, node] : graph) {
            nodeMap[pose] = ImVec2(static_cast<float>(pose.position.x) + 0.5, static_cast<float>(pose.position.y) + 0.5);

            switch (pose.orientation) {
                case micras::nav::RIGHT:
                    nodeMap[pose].x -= 0.25f;
                    break;
                case micras::nav::UP:
                    nodeMap[pose].y -= 0.25f;
                    break;
                case micras::nav::LEFT:
                    nodeMap[pose].x += 0.25f;
                    break;
                case micras::nav::DOWN:
                    nodeMap[pose].y += 0.25f;
                    break;
            }
        }

        // Draw edges with arrows
        for (const auto& [fromPose, fromNode] : graph) {
            for (const auto& [toPose, cost] : fromNode.next_costs) {
                // Draw main edge line
                float x_coords[2] = {nodeMap[fromPose].x, nodeMap[toPose].x};
                float y_coords[2] = {nodeMap[fromPose].y, nodeMap[toPose].y};
                ImPlot::PlotLine("Edges", x_coords, y_coords, 2);

                // Calculate direction vector
                float dx = x_coords[1] - x_coords[0];
                float dy = y_coords[1] - y_coords[0];

                // Normalize direction vector
                float length = std::sqrt(dx * dx + dy * dy);
                if (length > 0) {
                    dx /= length;
                    dy /= length;

                    // Arrow size (adjust as needed)
                    float arrow_size = 0.025f;

                    // Position arrow slightly before the end (80% along the edge)
                    float arrow_x = x_coords[0] + dx * length * 0.8f;
                    float arrow_y = y_coords[0] + dy * length * 0.8f;

                    // Create arrowhead (perpendicular to direction)
                    float arrow_dx = -dy;  // Perpendicular to direction
                    float arrow_dy = dx;   // Perpendicular to direction

                    // Draw the arrowhead as two lines
                    float arrow_x1[2] = {arrow_x, arrow_x - dx * arrow_size + arrow_dx * arrow_size};
                    float arrow_y1[2] = {arrow_y, arrow_y - dy * arrow_size + arrow_dy * arrow_size};
                    float arrow_x2[2] = {arrow_x, arrow_x - dx * arrow_size - arrow_dx * arrow_size};
                    float arrow_y2[2] = {arrow_y, arrow_y - dy * arrow_size - arrow_dy * arrow_size};

                    // Draw arrow lines
                    ImPlot::PlotLine("Edges", arrow_x1, arrow_y1, 2);
                    ImPlot::PlotLine("Edges", arrow_x2, arrow_y2, 2);
                }
            }
        }

        // Draw nodes as circles with orientation arrows
        for (const auto& [pose, point] : nodeMap) {
            float node_x = point.x;
            float node_y = point.y;

            // Draw orientation arrow inside the circle
            // Calculate arrow direction based on the node's orientation
            float dx = 0, dy = 0;
            float arrow_length = 0.04f;  // REDUCED Length of orientation arrow

            switch (pose.orientation) {
                case micras::nav::RIGHT:
                    dx = 1.0f;
                    dy = 0.0f;
                    break;
                case micras::nav::UP:
                    dx = 0.0f;
                    dy = 1.0f;  // Y is inverted in most plotting systems
                    break;
                case micras::nav::LEFT:
                    dx = -1.0f;
                    dy = 0.0f;
                    break;
                case micras::nav::DOWN:
                    dx = 0.0f;
                    dy = -1.0f;  // Y is inverted in most plotting systems
                    break;
            }

            // Draw the node as a scatter point (circle)
            ImPlot::PushStyleVar(ImPlotStyleVar_MarkerSize, 6);  // Larger circle
            ImPlot::PlotScatter("Nodes", &node_x, &node_y, 1);
            ImPlot::PopStyleVar();

            // Draw the orientation arrow
            float arrow_x[2] = {node_x, node_x + dx * arrow_length};
            float arrow_y[2] = {node_y, node_y + dy * arrow_length};

            // Use a different color for orientation arrows
            ImPlot::PushStyleColor(ImPlotCol_Line, ImVec4(0.8f, 0.0f, 0.0f, 1.0f));
            ImPlot::PlotLine("Orientation", arrow_x, arrow_y, 2);

            // Add a smaller arrowhead to the orientation arrow
            float ah_size = 0.1f;  // REDUCED arrowhead size
            float arrow_dx = -dy;  // Perpendicular to direction
            float arrow_dy = dx;   // Perpendicular to direction

            float ah_x1[2] = {arrow_x[1], arrow_x[1] - dx * ah_size + arrow_dx * ah_size};
            float ah_y1[2] = {arrow_y[1], arrow_y[1] - dy * ah_size + arrow_dy * ah_size};
            float ah_x2[2] = {arrow_x[1], arrow_x[1] - dx * ah_size - arrow_dx * ah_size};
            float ah_y2[2] = {arrow_y[1], arrow_y[1] - dy * ah_size - arrow_dy * ah_size};

            ImPlot::PlotLine("Orientation", ah_x1, ah_y1, 2);
            ImPlot::PlotLine("Orientation", ah_x2, ah_y2, 2);

            ImPlot::PopStyleColor();
        }

        ImPlot::EndPlot();
        ImPlot::PopStyleVar(1);
    }
}

void Plot::drawMazeCostHeatmap(micras::ProxyBridge& proxyBridge) {
    // Get maze dimensions and cell costs
    const int width = proxyBridge.get_maze_width();
    const int height = proxyBridge.get_maze_height();

    // Get costs from proxy bridge
    mazeCostData = proxyBridge.get_maze_cell_costs();

    // Find min/max values for the colormap scaling
    int16_t dataMin = std::numeric_limits<int16_t>::max();
    int16_t dataMax = std::numeric_limits<int16_t>::lowest();

    for (const auto& cost : mazeCostData) {
        dataMin = std::min(dataMin, cost);
        dataMax = std::max(dataMax, cost);
    }

    ImGui::Text("Maze Cost Heatmap");

    // Add options for different colormaps
    static int  selected_colormap = ImPlotColormap_Plasma;
    const char* colormap_names[] = {"Default", "Deep", "Dark",     "Pastel", "Paired", "Viridis", "Plasma",   "Hot",  "Cool",
                                    "Pink",    "Jet",  "Twilight", "RdBu",   "BrBG",   "PiYG",    "Spectral", "Greys"};

    ImGui::SameLine();
    ImGui::SetNextItemWidth(120);
    ImGui::Combo("Colormap", &selected_colormap, colormap_names, IM_ARRAYSIZE(colormap_names));

    // Display data range information
    ImGui::Text("Data range: [%d, %d]", dataMin, dataMax);
    ImGui::Text("Minimum cost: %d", proxyBridge.get_min_maze_cost());

    if (ImPlot::BeginPlot("##MazeHeatmap", ImVec2(-1, 400), ImPlotFlags_NoFrame | ImPlotFlags_NoLegend | ImPlotFlags_NoMouseText)) {
        ImPlot::SetupAxes("X", "Y", 0, ImPlotAxisFlags_Invert | ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_NoLabel);
        ImPlot::SetupAxisLimits(ImAxis_X1, 0, width, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1, 0, height, ImGuiCond_Always);

        // Apply the selected colormap
        ImPlot::PushColormap(selected_colormap);

        static int                      cmap = ImPlotColormap_Plasma;
        static ImPlotColormapScaleFlags flags = 0;

        // Plot the heatmap with the selected colormap and scale
        ImPlot::PlotHeatmap("Cost", mazeCostData.data(), height, width, dataMin, dataMax, "%d", ImPlotPoint(0, 0), ImPlotPoint(width, height));

        ImGui::SameLine();
        ImPlot::ColormapScale("##Scale", dataMin, dataMax, ImVec2(60, 400));

        ImPlot::PopColormap();
        ImPlot::EndPlot();
    }
}

void Plot::drawMazeCost3DSurface(micras::ProxyBridge& proxyBridge) {
    // Get maze dimensions and cost data
    const int width = proxyBridge.get_maze_width();
    const int height = proxyBridge.get_maze_height();

    if (mazeCostData.empty()) {
        mazeCostData = proxyBridge.get_maze_cell_costs();
    }

    ImGui::Text("Maze Cost 3D Surface");

    // Create grid arrays for the surface plot
    static std::vector<float> xs;
    static std::vector<float> ys;
    static std::vector<float> zs;

    // Resize arrays if needed
    if (xs.size() != width * height) {
        xs.resize(width * height);
        ys.resize(width * height);
        zs.resize(width * height);

        // Populate x and y coordinates
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                int idx = i * width + j;
                xs[idx] = static_cast<float>(j);
                ys[idx] = static_cast<float>(i);
            }
        }
    }

    // Update z values from maze cost data
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int idx = i * width + j;
            zs[idx] = mazeCostData[idx] * mazeCostColorScale;  // Apply color scale
        }
    }

    // Fill color options
    static int    selected_fill = 1;  // Colormap by default
    static ImVec4 solid_color = ImVec4(0.8f, 0.8f, 0.2f, 0.6f);
    const char*   colormaps[] = {"Viridis", "Plasma", "Hot", "Cool", "Pink", "Jet", "Twilight", "RdBu", "BrBG", "PiYG", "Spectral", "Greys"};
    static int    sel_colormap = 1;  // Viridis by default

    // Choose solid color or colormap
    ImGui::Text("Fill color:");
    ImGui::SameLine();
    ImGui::RadioButton("Solid", &selected_fill, 0);
    if (selected_fill == 0) {
        ImGui::SameLine();
        ImGui::ColorEdit4("##SurfaceSolidColor", (float*)&solid_color, ImGuiColorEditFlags_NoInputs);
    }

    ImGui::SameLine();
    ImGui::RadioButton("Colormap", &selected_fill, 1);
    if (selected_fill == 1) {
        ImGui::SameLine();
        ImGui::SetNextItemWidth(120);
        ImGui::Combo("##SurfaceColormap", &sel_colormap, colormaps, IM_ARRAYSIZE(colormaps));
    }

    // Enhanced zoom controls
    ImGui::SameLine(ImGui::GetWindowWidth() - 300);
    ImGui::Text("Zoom:");
    ImGui::SameLine();
    ImGui::SliderFloat("Scale", &mazeCostColorScale, 0.1f, 10.0f, "%.1f");

    // Custom range options with better controls
    static bool  custom_range = false;
    static float range_min = 0.0f;
    static float range_max = mazeCostColorScale;
    static float zoom_center = 0.5f;
    static float zoom_width = 1.0f;

    // Find actual min/max in current data
    float dataMin = std::numeric_limits<float>::max();
    float dataMax = std::numeric_limits<float>::lowest();
    for (const auto& z : zs) {
        dataMin = std::min(dataMin, z);
        dataMax = std::max(dataMax, z);
    }
    if (dataMin == std::numeric_limits<float>::max()) {
        dataMin = 0.0f;
        dataMax = 1.0f;
    }

    ImGui::Checkbox("Custom Z range", &custom_range);

    if (custom_range) {
        // More intuitive zoom controls
        ImGui::SetNextItemWidth(150);
        ImGui::SliderFloat("Min", &range_min, dataMin, range_max - 0.01f);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(150);
        ImGui::SliderFloat("Max", &range_max, range_min + 0.01f, dataMax);

        // Add zoom controls
        ImGui::SetNextItemWidth(150);
        if (ImGui::SliderFloat("Zoom center", &zoom_center, 0.0f, 1.0f)) {
            // Map zoom center (0-1) to the data range
            float center = dataMin + zoom_center * (dataMax - dataMin);
            float halfWidth = zoom_width * (dataMax - dataMin) / 2.0f;
            range_min = center - halfWidth;
            range_max = center + halfWidth;

            // Clamp to data range
            range_min = std::max(range_min, dataMin);
            range_max = std::min(range_max, dataMax);
        }

        ImGui::SameLine();
        ImGui::SetNextItemWidth(150);
        if (ImGui::SliderFloat("Zoom width", &zoom_width, 0.01f, 1.0f)) {
            // Recalculate range based on center and width
            float center = dataMin + zoom_center * (dataMax - dataMin);
            float halfWidth = zoom_width * (dataMax - dataMin) / 2.0f;
            range_min = center - halfWidth;
            range_max = center + halfWidth;

            // Clamp to data range
            range_min = std::max(range_min, dataMin);
            range_max = std::min(range_max, dataMax);
        }

        // Add some helpful info about data range
        ImGui::Text("Data range: [%.3f, %.3f]", dataMin, dataMax);
    } else {
        // If not using custom range, use the full data range
        range_min = dataMin;
        range_max = dataMax;
    }

    // Begin the plot with appropriate colormap
    if (selected_fill == 1) {
        switch (sel_colormap) {
            case 0:
                ImPlot3D::PushColormap(ImPlot3DColormap_Viridis);
                break;
            case 1:
                ImPlot3D::PushColormap(ImPlot3DColormap_Plasma);
                break;
            case 2:
                ImPlot3D::PushColormap(ImPlot3DColormap_Hot);
                break;
            case 3:
                ImPlot3D::PushColormap(ImPlot3DColormap_Cool);
                break;
            case 4:
                ImPlot3D::PushColormap(ImPlot3DColormap_Pink);
                break;
            case 5:
                ImPlot3D::PushColormap(ImPlot3DColormap_Jet);
                break;
            case 6:
                ImPlot3D::PushColormap(ImPlot3DColormap_Twilight);
                break;
            case 7:
                ImPlot3D::PushColormap(ImPlot3DColormap_RdBu);
                break;
            case 8:
                ImPlot3D::PushColormap(ImPlot3DColormap_BrBG);
                break;
            case 9:
                ImPlot3D::PushColormap(ImPlot3DColormap_PiYG);
                break;
            case 10:
                ImPlot3D::PushColormap(ImPlot3DColormap_Spectral);
                break;
            case 11:
                ImPlot3D::PushColormap(ImPlot3DColormap_Greys);
                break;
            default:
                ImPlot3D::PushColormap(ImPlot3DColormap_Viridis);
        }
    }

    if (ImPlot3D::BeginPlot("##MazeSurface3D", ImVec2(-1, 350), ImPlot3DFlags_NoClip)) {
        // Configure axes
        ImPlot3D::SetupAxes("X", "Y", "Cost");
        ImPlot3D::SetupAxisLimits(ImAxis3D_X, 0, width, ImGuiCond_Always);
        ImPlot3D::SetupAxisLimits(ImAxis3D_Y, 0, height, ImGuiCond_Always);
        ImPlot3D::SetupAxisLimits(ImAxis3D_Z, range_min, range_max, ImGuiCond_Always);

        // Set styles
        ImPlot3D::PushStyleVar(ImPlot3DStyleVar_LineWeight, 1.0f);
        ImPlot3D::PushStyleVar(ImPlot3DStyleVar_FillAlpha, 0.8f);

        if (selected_fill == 0) {
            ImPlot3D::SetNextFillStyle(solid_color);
        }

        // Plot the surface
        if (xs.size() > 0 && ys.size() > 0 && zs.size() > 0) {
            ImPlot3D::PlotSurface("Maze Cost Surface", xs.data(), ys.data(), zs.data(), width, height, range_min, range_max);
        }

        // End the plot
        ImPlot3D::PopStyleVar(2);  // Pop LineWeight and FillAlpha
        ImPlot3D::EndPlot();
    }

    if (selected_fill == 1) {
        ImPlot3D::PopColormap();
    }
}

void Plot::destroy() {
    ImPlot::DestroyContext();
    ImPlot3D::DestroyContext();
}

void Plot::initPlotVariables(micrasverse::physics::Box2DMicrasBody& micrasBody, micras::ProxyBridge& proxyBridge) {
    if (variablesInitialized) {
        return;
    }

    // Create all available plot variables with unique colors
    plotVariables.push_back(PlotVariable("rightMotorCurrent", "Right Motor Current (A)", ImVec4(1.0f, 0.0f, 0.0f, 1.0f)));
    plotVariables.push_back(PlotVariable("leftMotorCurrent", "Left Motor Current (A)", ImVec4(0.0f, 1.0f, 0.0f, 1.0f)));
    plotVariables.push_back(PlotVariable("rightMotorAngVel", "Right Motor Angular Velocity (rad/s)", ImVec4(0.0f, 0.0f, 1.0f, 1.0f)));
    plotVariables.push_back(PlotVariable("leftMotorAngVel", "Left Motor Angular Velocity (rad/s)", ImVec4(1.0f, 1.0f, 0.0f, 1.0f)));
    plotVariables.push_back(PlotVariable("bodyAngVel", "Body Angular Velocity (rad/s)", ImVec4(1.0f, 0.0f, 1.0f, 1.0f)));
    plotVariables.push_back(PlotVariable("bodyLinVel", "Body Linear Velocity (m/s)", ImVec4(0.0f, 1.0f, 1.0f, 1.0f)));
    plotVariables.push_back(PlotVariable("linearAccel", "Linear Acceleration (m/s²)", ImVec4(0.5f, 0.5f, 0.5f, 1.0f)));
    plotVariables.push_back(PlotVariable("proxyLinSpeed", "Proxy Linear Speed", ImVec4(0.8f, 0.2f, 0.2f, 1.0f)));
    plotVariables.push_back(PlotVariable("proxyAngSpeed", "Proxy Angular Speed", ImVec4(0.2f, 0.8f, 0.2f, 1.0f)));
    plotVariables.push_back(PlotVariable("linearSpeed", "Linear Speed", ImVec4(0.2f, 0.2f, 0.8f, 1.0f)));
    plotVariables.push_back(PlotVariable("wallSensor0", "Wall Sensor 0", ImVec4(0.7f, 0.3f, 0.3f, 1.0f)));
    plotVariables.push_back(PlotVariable("wallSensor1", "Wall Sensor 1", ImVec4(0.3f, 0.7f, 0.3f, 1.0f)));
    plotVariables.push_back(PlotVariable("wallSensor2", "Wall Sensor 2", ImVec4(0.3f, 0.3f, 0.7f, 1.0f)));
    plotVariables.push_back(PlotVariable("wallSensor3", "Wall Sensor 3", ImVec4(0.7f, 0.7f, 0.3f, 1.0f)));
    plotVariables.push_back(PlotVariable("rightAppliedForce", "Right Applied Force (N)", ImVec4(0.7f, 0.3f, 0.7f, 1.0f)));
    plotVariables.push_back(PlotVariable("leftAppliedForce", "Left Applied Force (N)", ImVec4(0.3f, 0.7f, 0.7f, 1.0f)));
    plotVariables.push_back(PlotVariable("linearPIDSetpoint", "Linear PID Setpoint", ImVec4(0.9f, 0.4f, 0.0f, 1.0f)));
    plotVariables.push_back(PlotVariable("angularPIDSetpoint", "Angular PID Setpoint", ImVec4(0.0f, 0.4f, 0.9f, 1.0f)));
    plotVariables.push_back(PlotVariable("linearPIDResponse", "Linear PID Response", ImVec4(0.9f, 0.0f, 0.4f, 1.0f)));
    plotVariables.push_back(PlotVariable("angularPIDResponse", "Angular PID Response", ImVec4(0.4f, 0.9f, 0.0f, 1.0f)));
    plotVariables.push_back(PlotVariable("leftFeedForward", "Left Feed Forward Response", ImVec4(0.5f, 0.5f, 0.0f, 1.0f)));
    plotVariables.push_back(PlotVariable("rightFeedForward", "Right Feed Forward Response", ImVec4(0.0f, 0.5f, 0.5f, 1.0f)));
    plotVariables.push_back(PlotVariable("linearIntegrative", "Linear Integrative Response", ImVec4(0.5f, 0.0f, 0.5f, 1.0f)));
    plotVariables.push_back(PlotVariable("angularIntegrative", "Angular Integrative Response", ImVec4(0.9f, 0.9f, 0.0f, 1.0f)));

    // Default selected variables for each chart (to avoid empty charts initially)
    for (int i = 0; i < 5; i++) {
        if (i < plotVariables.size()) {
            selectedVariablesForCharts[i].push_back(i);
        }
    }

    // Make sure the drag rectangle is positioned within the visible window initially
    // Position it in the latter half of the visible window with a reasonable height
    float rectWidth = history * 0.25f;  // 25% of visible range
    dragRect.X.Min = t - rectWidth - 0.1f * history;
    dragRect.X.Max = t - 0.1f * history;
    dragRect.Y.Min = -1.0f;
    dragRect.Y.Max = 1.0f;

    variablesInitialized = true;
}

void Plot::updatePlotVariables(micrasverse::physics::Box2DMicrasBody& micrasBody, micras::ProxyBridge& proxyBridge) {
    float deltaTime = ImGui::GetIO().DeltaTime;
    t += deltaTime;

    for (auto& var : plotVariables) {
        float value = 0.0f;

        // Get value based on variable name
        if (var.name == "rightMotorCurrent")
            value = micrasBody.getRightMotor().getCurrent();
        else if (var.name == "leftMotorCurrent")
            value = micrasBody.getLeftMotor().getCurrent();
        else if (var.name == "rightMotorAngVel")
            value = micrasBody.getRightMotor().getAngularVelocity();
        else if (var.name == "leftMotorAngVel")
            value = micrasBody.getLeftMotor().getAngularVelocity();
        else if (var.name == "bodyAngVel")
            value = micrasBody.getRightMotor().getBodyAngularVelocity();
        else if (var.name == "bodyLinVel")
            value = micrasBody.getLeftMotor().getBodyLinearVelocity();
        else if (var.name == "linearAccel")
            value = micrasBody.getLinearAcceleration();
        else if (var.name == "proxyLinSpeed")
            value = proxyBridge.get_linear_speed();
        else if (var.name == "proxyAngSpeed")
            value = proxyBridge.get_angular_speed();
        else if (var.name == "linearSpeed")
            value = micrasBody.getLinearSpeed();
        else if (var.name == "wallSensor0")
            value = proxyBridge.get_wall_sensor_reading(0);
        else if (var.name == "wallSensor1")
            value = proxyBridge.get_wall_sensor_reading(1);
        else if (var.name == "wallSensor2")
            value = -proxyBridge.get_wall_sensor_reading(2);
        else if (var.name == "wallSensor3")
            value = proxyBridge.get_wall_sensor_reading(3);
        else if (var.name == "rightAppliedForce")
            value = micrasBody.getRightMotor().getAppliedForce();
        else if (var.name == "leftAppliedForce")
            value = micrasBody.getLeftMotor().getAppliedForce();
        else if (var.name == "linearPIDSetpoint")
            value = proxyBridge.get_linear_pid_setpoint();
        else if (var.name == "angularPIDSetpoint")
            value = proxyBridge.get_angular_pid_setpoint();
        else if (var.name == "linearPIDResponse")
            value = proxyBridge.get_linear_pid_last_response();
        else if (var.name == "angularPIDResponse")
            value = proxyBridge.get_angular_pid_last_response();
        else if (var.name == "leftFeedForward")
            value = proxyBridge.get_left_feed_forward_response();
        else if (var.name == "rightFeedForward")
            value = proxyBridge.get_right_feed_forward_response();
        else if (var.name == "linearIntegrative")
            value = proxyBridge.get_linear_integrative_response();
        else if (var.name == "angularIntegrative")
            value = proxyBridge.get_angular_integrative_response();

        var.data.push_back(ImVec2(t, value));

        // Limit data size
        if (var.data.size() > 2000) {
            var.data.erase(var.data.begin());
        }
    }
}

}  // namespace micrasverse::render
