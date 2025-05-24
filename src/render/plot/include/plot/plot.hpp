#ifndef PLOT_HPP
#define PLOT_HPP

#include "physics/box2d_micrasbody.hpp"
#include "micras/proxy/proxy_bridge.hpp"
#include "micras/nav/grid_pose.hpp"
#include "micras/nav/maze_graph.hpp"

#include "imgui.h"
#include "implot.h"
#include "implot3d.h"

namespace micrasverse::render {

class Plot {
public:
    struct Node {
        int               id;
        float             x, y;
        micras::nav::Side orientation;

        // Constructor from GridPose and cell size
        Node(int id, const micras::nav::GridPose& pose, float cell_size) : id(id), orientation(pose.orientation) {
            // Convert GridPoint to world coordinates
            auto vec = pose.position.to_vector(cell_size);
            x = vec.x;
            y = vec.y;
        }

        // Default constructor
        Node(int id, float x, float y, micras::nav::Side orientation) : id(id), x(x), y(y), orientation(orientation) { }
    };

    struct Edge {
        int from_node_id;
        int to_node_id;
    };

    void drawGraph(std::unordered_map<micras::nav::GridPose, micras::nav::MazeGraph::Node> graph);

    struct ScrollingBuffer {
        int              maxSize;
        int              offset;
        ImVector<ImVec2> data;

        ScrollingBuffer(int maxSize = 2000);

        void addPoint(const float x, const float y);
        void erase();
    };

    struct RollingBuffer {
        int              span;
        ImVector<ImVec2> data;

        RollingBuffer(int span = 10, int maxSize = 2000);

        void addPoint(const float x, const float y);
    };

    struct PlotVariable {
        std::string      name;
        std::string      label;
        ImVector<ImVec2> data;
        ImVec4           color;
        bool             selected;

        PlotVariable(const std::string& name, const std::string& label, const ImVec4& color);
    };

    Plot();

    void init();

    void draw(micrasverse::physics::Box2DMicrasBody& micrasBody, micras::ProxyBridge& proxyBridge, bool simulationIsPaused = false);

    void drawDragAndDrop(micrasverse::physics::Box2DMicrasBody& micrasBody, micras::ProxyBridge& proxyBridge, bool simulationIsPaused = false);

    void drawMazeCostHeatmap(micras::ProxyBridge& proxyBridge);

    void drawMazeCost3DSurface(micras::ProxyBridge& proxyBridge);

    void destroy();

    bool  showPlots;
    bool  showDragAndDropMode;
    bool  showMazeCostVisualizations;
    float updateInterval;
    float timeSinceLastUpdate;
    float history;  // Time history window in seconds

    // Drag rectangle
    ImPlotRect dragRect;
    bool       dragRectClicked;
    bool       dragRectHovered;
    bool       dragRectHeld;

private:
    void initPlotVariables(micrasverse::physics::Box2DMicrasBody& micrasBody, micras::ProxyBridge& proxyBridge);
    void updatePlotVariables(micrasverse::physics::Box2DMicrasBody& micrasBody, micras::ProxyBridge& proxyBridge);

    std::vector<PlotVariable> plotVariables;
    std::vector<int>          selectedVariablesForCharts[5];
    float                     t;
    bool                      variablesInitialized;

    // Maze visualization data
    std::vector<int16_t> mazeCostData;
    float                mazeCostColorScale;
    bool                 mazeCostVisualizationInitialized;
};

}  // namespace micrasverse::render

#endif  // PLOT_HPP
