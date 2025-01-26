#include "GUI/plot.hpp"

#include "implot.h"

#include <cmath>

namespace micrasverse::render {

Plot::ScrollingBuffer::ScrollingBuffer(int maxSize) {
    this->maxSize = maxSize;
    this->offset = 0;
    this->data.reserve(maxSize);
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
    this->data.reserve(maxSize);
}

void Plot::RollingBuffer::addPoint(float x, float y) {
    float xmod = std::fmodf(x, this->span);

    if (!this->data.empty() && xmod < this->data.back().x) {
        this->data.shrink(0);
    }

    this->data.push_back(ImVec2(xmod, y));
}

Plot::Plot() {}

void Plot::init() {
    ImPlot::CreateContext();
}

void Plot::draw(micrasverse::physics::MicrasBody& micrasBody){
    static ScrollingBuffer sdata1, sdata2;
    static ScrollingBuffer rdata1, rdata2, rdata3, rdata4, rdata5, rdata6, rdata7, rdata8, rdata9, rdata10, rdata11, rdata12;

    static float t = 0;
    t += ImGui::GetIO().DeltaTime;
    
    sdata1.addPoint(t, micrasBody.motors[0].getCurrent());
    sdata2.addPoint(t, micrasBody.motors[1].getCurrent());

    rdata1.addPoint(t, micrasBody.motors[0].getAngularVelocity());
    rdata2.addPoint(t, micrasBody.motors[1].getAngularVelocity());

    rdata3.addPoint(t, micrasBody.motors[0].bodyAngularVelocity);
    rdata4.addPoint(t, micrasBody.motors[0].bodyLinearVelocity);
    rdata5.addPoint(t, micrasBody.linearAcceleration);

    rdata7.addPoint(t, micrasBody.wallSensors.get_sensors()[0].getReading());
    rdata8.addPoint(t, micrasBody.wallSensors.get_sensors()[1].getReading());
    rdata9.addPoint(t, micrasBody.wallSensors.get_sensors()[2].getReading());
    rdata10.addPoint(t, micrasBody.wallSensors.get_sensors()[3].getReading());
    rdata11.addPoint(t, micrasBody.motors[0].appliedForce);
    rdata12.addPoint(t, micrasBody.motors[1].appliedForce);

    static float history = 5.0f;
    ImGui::SliderFloat("History",&history,1,30,"%.1f s");

    static ImPlotAxisFlags flags = ImPlotAxisFlags_NoInitialFit;
    
    if (ImPlot::BeginPlot("##DC motors electrical current (A)", ImVec2(-1,100))) {
        ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoTickLabels, flags);
        ImPlot::SetupAxisLimits(ImAxis_X1,t - history, t, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1,-2.0f,+2.0f);
        ImPlot::PlotLine("Motor 0 electrical current (A)", &sdata1.data[0].x, &sdata1.data[0].y, sdata1.data.size(), 0, sdata1.offset, 2*sizeof(float));
        ImPlot::PlotLine("Motor 1 electrical current (A)", &sdata2.data[0].x, &sdata2.data[0].y, sdata2.data.size(), 0, sdata2.offset, 2*sizeof(float));
        ImPlot::EndPlot();
    }
    if (ImPlot::BeginPlot("##DC motors' angular velocity (rad/s)", ImVec2(-1,100))) {
        ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoTickLabels, flags);
        ImPlot::SetupAxisLimits(ImAxis_X1,t - history, t, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1,-150,150);
        ImPlot::PlotLine("Motor 0 angular velocity (rad/s)", &rdata1.data[0].x, &rdata1.data[0].y, rdata1.data.size(), 0, rdata1.offset, 2 * sizeof(float));
        ImPlot::PlotLine("Motor 1 angular velocity (rad/s)", &rdata2.data[0].x, &rdata2.data[0].y, rdata2.data.size(), 0, rdata2.offset, 2 * sizeof(float));
        ImPlot::EndPlot();
    }    
    if (ImPlot::BeginPlot("##Micras' angular velocity (rad/s)", ImVec2(-1,100))) {
        ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoTickLabels, flags);
        ImPlot::SetupAxisLimits(ImAxis_X1,t - history, t, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1,-20,20);
        ImPlot::PlotLine("Micras' angular velocity (rad/s)", &rdata3.data[0].x, &rdata3.data[0].y, rdata3.data.size(), 0, rdata3.offset, 2 * sizeof(float));
        ImPlot::EndPlot();
    }         
    if (ImPlot::BeginPlot("##Micras' linear velocity (m/s)", ImVec2(-1,100))) {
        ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoTickLabels, flags);
        ImPlot::SetupAxisLimits(ImAxis_X1,t - history, t, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1,-1.2,1.2);
        ImPlot::PlotLine("Micras' linear velocity (m/s)", &rdata4.data[0].x, &rdata4.data[0].y, rdata4.data.size(), 0, rdata4.offset, 2 * sizeof(float));
        ImPlot::EndPlot();
    }         
    if (ImPlot::BeginPlot("##Micras' linear acceleration (m/s²)", ImVec2(-1,100))) {
        ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoTickLabels, flags);
        ImPlot::SetupAxisLimits(ImAxis_X1,t - history, t, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1,-20,20);
        ImPlot::PlotLine("Micras' linear acceleration (m/s²)", &rdata5.data[0].x, &rdata5.data[0].y, rdata5.data.size(), 0, rdata5.offset, 2 * sizeof(float));
        ImPlot::EndPlot();
    }
    if (ImPlot::BeginPlot("##Micras' applied force (N)", ImVec2(-1,100))) {
        ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoTickLabels, flags);
        ImPlot::SetupAxisLimits(ImAxis_X1,t - history, t, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1,-0.2,0.2);
        ImPlot::PlotLine("Micras' applied force motor 0 (N)", &rdata11.data[0].x, &rdata11.data[0].y, rdata11.data.size(), 0, rdata11.offset, 2 * sizeof(float));
        ImPlot::PlotLine("Micras' applied force motor 1 (N)", &rdata12.data[0].x, &rdata12.data[0].y, rdata12.data.size(), 0, rdata12.offset, 2 * sizeof(float));
        ImPlot::EndPlot();
    }
    /*
    if (ImPlot::BeginPlot("##Micras' front distance sensors reading", ImVec2(-1,100))) {
        ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoTickLabels, flags);
        ImPlot::SetupAxisLimits(ImAxis_X1,t - history, t, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1,-0.8,0.8);
        ImPlot::PlotLine("Front distance sensor 1", &rdata7.data[0].x, &rdata7.data[0].y, rdata7.data.size(), 0, rdata7.offset, 2 * sizeof(float));
        ImPlot::PlotLine("Front distance sensor 2", &rdata8.data[0].x, &rdata8.data[0].y, rdata8.data.size(), 0, rdata8.offset, 2 * sizeof(float));
        ImPlot::EndPlot();
    }
    if (ImPlot::BeginPlot("##Micras' side distance sensors reading", ImVec2(-1,100))) {
        ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoTickLabels, flags);
        ImPlot::SetupAxisLimits(ImAxis_X1,t - history, t, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1,-0.8,0.8);
        ImPlot::PlotLine("Side distance sensor 1", &rdata9.data[0].x, &rdata9.data[0].y, rdata9.data.size(), 0, rdata9.offset, 2 * sizeof(float));
        ImPlot::PlotLine("Side distance sensor 2", &rdata10.data[0].x, &rdata10.data[0].y, rdata10.data.size(), 0, rdata10.offset, 2 * sizeof(float));
        ImPlot::EndPlot();
    }*/


}

void Plot::destroy() {
    ImPlot::DestroyContext();
}


} // namespace micrasverse::render
