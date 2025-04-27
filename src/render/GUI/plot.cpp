#include "GUI/plot.hpp"
#include "physics/box2d_motor.hpp"
#include "physics/box2d_distance_sensor.hpp"
#include "implot.h"

#include <cmath>

namespace micrasverse::render {

Plot::ScrollingBuffer::ScrollingBuffer(int maxSize) {
    this->maxSize = maxSize;
    this->offset = 0;
}

void Plot::ScrollingBuffer::addPoint(const float x, const float y) {
    if (this->data.size() < this->maxSize)
        this->data.push_back(ImVec2(x,y));
    else {
        this->data[this->offset] = ImVec2(x,y);
        this->offset =  (this->offset + 1) % this->maxSize;
    }
}

void Plot::ScrollingBuffer::erase() {
    if (this->data.size() > 0) {
        this->data.shrink(0);
        this->offset  = 0;
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
    updateInterval = 0.1f;
    timeSinceLastUpdate = 0.0f;
}

void Plot::init() {
    ImPlot::CreateContext();
}

void Plot::draw(micrasverse::physics::Box2DMicrasBody& micrasBody, micras::ProxyBridge& proxyBridge, bool simulationIsPaused){  
    if (!showPlots) {
        return;
    }
    
    static ScrollingBuffer sdata1, sdata2, sdata3, sdata4, sdata5, sdata6, sdata7, sdata8, sdata9, sdata10;
    static ScrollingBuffer rdata1, rdata2, rdata3, rdata4, rdata5, rdata6, rdata7, rdata8, rdata9, rdata10,
                           rdata11, rdata12, rdata13, rdata14, rdata15, rdata16, rdata17, rdata18, rdata19,
                           rdata20, rdata21, rdata22, rdata23, rdata24, rdata25, rdata26, rdata27, rdata28, rdata29, rdata30;
    
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

    // Control for plot history
    static float history = 30.0f;
    ImGui::SliderFloat("History", &history, 1, 30, "%.1f s");

    static ImPlotAxisFlags flags = ImPlotAxisFlags_NoInitialFit;

    /*
    // Only render plots if there's data
    if (!sdata1.data.empty() && ImPlot::BeginPlot("##DC motors electrical current (A)", ImVec2(-1,100))) {
        ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoTickLabels, flags);
        ImPlot::SetupAxisLimits(ImAxis_X1,t - history, t, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1,-2.0f,+2.0f);
        ImPlot::PlotLine("Motor 0 electrical current (A)", &sdata1.data[0].x, &sdata1.data[0].y, sdata1.data.size(), 0, sdata1.offset, 2*sizeof(float));
        ImPlot::PlotLine("Motor 1 electrical current (A)", &sdata2.data[0].x, &sdata2.data[0].y, sdata2.data.size(), 0, sdata2.offset, 2*sizeof(float));
        ImPlot::EndPlot();
    }
    if (!rdata1.data.empty() && ImPlot::BeginPlot("##DC motors' angular velocity (rad/s)", ImVec2(-1,100))) {
        ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoTickLabels, flags);
        ImPlot::SetupAxisLimits(ImAxis_X1,t - history, t, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1,-150,150);
        ImPlot::PlotLine("Motor 0 angular velocity (rad/s)", &rdata1.data[0].x, &rdata1.data[0].y, rdata1.data.size(), 0, rdata1.offset, 2 * sizeof(float));
        ImPlot::PlotLine("Motor 1 angular velocity (rad/s)", &rdata2.data[0].x, &rdata2.data[0].y, rdata2.data.size(), 0, rdata2.offset, 2 * sizeof(float));
        ImPlot::EndPlot();
    }    
    if (!rdata3.data.empty() && ImPlot::BeginPlot("##Micras' angular velocity (rad/s)", ImVec2(-1,100))) {
        ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoTickLabels, flags);
        ImPlot::SetupAxisLimits(ImAxis_X1,t - history, t, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1,-20,20);
        ImPlot::PlotLine("Micras' angular velocity (rad/s)", &rdata3.data[0].x, &rdata3.data[0].y, rdata3.data.size(), 0, rdata3.offset, 2 * sizeof(float));
        ImPlot::EndPlot();
    }         
    if (!rdata4.data.empty() && ImPlot::BeginPlot("##Micras' linear velocity (m/s)", ImVec2(-1,100))) {
        ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoTickLabels, flags);
        ImPlot::SetupAxisLimits(ImAxis_X1,t - history, t, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1,-1.2,1.2);
        ImPlot::PlotLine("Micras' linear velocity (m/s)", &rdata4.data[0].x, &rdata4.data[0].y, rdata4.data.size(), 0, rdata4.offset, 2 * sizeof(float));
        ImPlot::EndPlot();
    }         
    if (!rdata5.data.empty() && ImPlot::BeginPlot("##Micras' linear acceleration (m/s²)", ImVec2(-1,100))) {
        ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoTickLabels, flags);
        ImPlot::SetupAxisLimits(ImAxis_X1,t - history, t, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1,-20,20);
        ImPlot::PlotLine("Micras' linear acceleration (m/s²)", &rdata5.data[0].x, &rdata5.data[0].y, rdata5.data.size(), 0, rdata5.offset, 2 * sizeof(float));
        ImPlot::EndPlot();
    }
    if (!rdata11.data.empty() && ImPlot::BeginPlot("##Micras' applied force (N)", ImVec2(-1,100))) {
        ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoTickLabels, flags);
        ImPlot::SetupAxisLimits(ImAxis_X1,t - history, t, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1,-0.2,0.2);
        ImPlot::PlotLine("Micras' applied force motor 0 (N)", &rdata11.data[0].x, &rdata11.data[0].y, rdata11.data.size(), 0, rdata11.offset, 2 * sizeof(float));
        ImPlot::PlotLine("Micras' applied force motor 1 (N)", &rdata12.data[0].x, &rdata12.data[0].y, rdata12.data.size(), 0, rdata12.offset, 2 * sizeof(float));
        ImPlot::EndPlot();
    }*/
    /*
    if (ImPlot::BeginPlot("##Micras' front distance sensors reading", ImVec2(-1,100), ImPlotFlags_NoFrame | ImPlotFlags_NoTitle)) {
        ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoTickLabels, flags);
        ImPlot::SetupAxisLimits(ImAxis_X1,t - history, t, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1,-100,1100);
        ImPlot::PlotLine("0", &rdata7.data[0].x, &rdata7.data[0].y, rdata7.data.size(), 0, rdata7.offset, 2 * sizeof(float));
        ImPlot::PlotLine("1", &rdata10.data[0].x, &rdata10.data[0].y, rdata10.data.size(), 0, rdata10.offset, 2 * sizeof(float));
        ImPlot::EndPlot();
    }
    if (ImPlot::BeginPlot("##Micras' side distance sensors reading", ImVec2(-1,100), ImPlotFlags_NoFrame | ImPlotFlags_NoTitle)) {
        ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoTickLabels, flags);
        ImPlot::SetupAxisLimits(ImAxis_X1,t - history, t, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1,-1100,1100);
        ImPlot::PlotLine("2", &rdata8.data[0].x, &rdata8.data[0].y, rdata8.data.size(), 0, rdata8.offset, 2 * sizeof(float));
        ImPlot::PlotLine("3", &rdata9.data[0].x, &rdata9.data[0].y, rdata9.data.size(), 0, rdata9.offset, 2 * sizeof(float));
        ImPlot::EndPlot();
    }*/

    if (!sdata1.data.empty() && ImPlot::BeginPlot("Linear PID setpoint", ImVec2(-1,150), ImPlotFlags_NoFrame | ImPlotFlags_NoTitle)) {
        ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoTickLabels, flags);
        ImPlot::SetupAxisLimits(ImAxis_X1,t - history, t, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1,-0.25,0.25);
        ImPlot::PlotLine("Linear PID setpoint", &rdata13.data[0].x, &rdata13.data[0].y, rdata13.data.size(), 0, rdata13.offset, 2*sizeof(float));
        ImPlot::PlotLine("Linear speed", &rdata6.data[0].x, &rdata6.data[0].y, rdata6.data.size(), 0, rdata6.offset, 2*sizeof(float));
        ImPlot::PlotLine("Body linear speed", &rdata20.data[0].x, &rdata20.data[0].y, rdata20.data.size(), 0, rdata20.offset, 2*sizeof(float));
        ImPlot::EndPlot();
    }

    if (!sdata1.data.empty() && ImPlot::BeginPlot("Angular PID setpoint", ImVec2(-1,150), ImPlotFlags_NoFrame | ImPlotFlags_NoTitle)) {
        ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoTickLabels, flags);
        ImPlot::SetupAxisLimits(ImAxis_X1,t - history, t, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1,-5,5);
        ImPlot::PlotLine("Angular PID setpoint", &rdata14.data[0].x, &rdata14.data[0].y, rdata14.data.size(), 0, rdata14.offset, 2*sizeof(float));
        ImPlot::PlotLine("Angular speed", &rdata19.data[0].x, &rdata19.data[0].y, rdata19.data.size(), 0, rdata19.offset, 2*sizeof(float));
        ImPlot::EndPlot();
    }

    if (!sdata1.data.empty() && ImPlot::BeginPlot("Linear PID response", ImVec2(-1,150), ImPlotFlags_NoFrame | ImPlotFlags_NoTitle)) {
        ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoTickLabels, flags);
        ImPlot::SetupAxisLimits(ImAxis_X1,t - history, t, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1,-1.5,1.5);
        ImPlot::PlotLine("Linear PID response", &rdata15.data[0].x, &rdata15.data[0].y, rdata15.data.size(), 0, rdata15.offset, 2*sizeof(float));
        ImPlot::PlotLine("Linear integrative response", &rdata21.data[0].x, &rdata21.data[0].y, rdata21.data.size(), 0, rdata21.offset, 2*sizeof(float));
        ImPlot::EndPlot();
    }

    if (!sdata1.data.empty() && ImPlot::BeginPlot("Angular PID response", ImVec2(-1,150), ImPlotFlags_NoFrame | ImPlotFlags_NoTitle)) {
        ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoTickLabels, flags);
        ImPlot::SetupAxisLimits(ImAxis_X1,t - history, t, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1,-5,5);
        ImPlot::PlotLine("Angular PID response", &rdata16.data[0].x, &rdata16.data[0].y, rdata16.data.size(), 0, rdata16.offset, 2*sizeof(float));
        ImPlot::PlotLine("Angular integrative response", &rdata22.data[0].x, &rdata22.data[0].y, rdata22.data.size(), 0, rdata22.offset, 2*sizeof(float));
        ImPlot::EndPlot();
    }

    if (!sdata1.data.empty() && ImPlot::BeginPlot("Feed forward response", ImVec2(-1,150), ImPlotFlags_NoFrame | ImPlotFlags_NoTitle)) {
        ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoTickLabels, flags);
        ImPlot::SetupAxisLimits(ImAxis_X1,t - history, t, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1,-20,20);
        ImPlot::PlotLine("Left feed forward response", &rdata17.data[0].x, &rdata17.data[0].y, rdata17.data.size(), 0, rdata17.offset, 2*sizeof(float));
        ImPlot::PlotLine("Right feed forward response", &rdata18.data[0].x, &rdata18.data[0].y, rdata18.data.size(), 0, rdata18.offset, 2*sizeof(float));
        ImPlot::EndPlot();
    }
}

void Plot::destroy() {
    ImPlot::DestroyContext();
}

} // namespace micrasverse::render
