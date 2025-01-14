#include "core/constants.h"
#include "physics/world.h"
#include "physics/maze.h"
#include "physics/rectanglebody.h"
#include "render/shader.h"
#include "render/camera.h"
#include "render/screen.h"
#include "render/keyboard.h"
#include "models/rectangle.h"
#include "models/micrasmodel.h"

#include "box2d/box2d.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"
#define GLFW_INCLUDE_NONE   // GLFW include guard
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>

int main() {
    static float deltaTime = 0.0f;
    static float lastFrame = 0.0f;

    /***************
     *** PHYSICS ***
     ***************/

    // Create box2d world
    micrasverse::physics::World world(micrasverse::GRAVITY);

    // Create maze
    micrasverse::physics::Maze maze(world.getWorldId());

    // Load maze design from file
    // ** To-do: make possible to load different mazes from file (and through GUI) during the simulation
    maze.loadFromFile("../external/mazefiles/classic/br2024-robochallenge-day3.txt");
    
    // Create Box2d objects for walls and lattice points
    maze.createBox2dObjects();

    // Create Box2d object for Micras
    // ** To-do: create a MicrasBody class that inherits from RectangleBody 
    micrasverse::physics::RectangleBody micrasBody(
        world.getWorldId(),
        (b2Vec2){(micrasverse::CELL_SIZE+micrasverse::WALL_THICKNESS)/2.0f, (micrasverse::CELL_SIZE+micrasverse::WALL_THICKNESS)/2.0f},                     // Initial position (center of the cell 0,0)
        (b2Vec2){micrasverse::MICRAS_WIDTH, micrasverse::MICRAS_HEIGHT},                                                                                    // Size
        b2_dynamicBody,                                                                                                                                     // Box2d body type (dynamic = afected by forces)
        (b2MassData){
            micrasverse::MICRAS_MASS,                                                                                                                       // Mass
            micrasverse::MICRAS_MASS*(micrasverse::MICRAS_HEIGHT*micrasverse::MICRAS_HEIGHT + micrasverse::MICRAS_WIDTH*micrasverse::MICRAS_WIDTH)/12.0f},  // Rotational inertia computed as m*(h² + w²)/12 for a rectangle
        micrasverse::MICRAS_RESTITUTION,                                                                                                                    // Restitution coefficient (how much it should bouce back)
        micrasverse::MICRAS_FRICTION                                                                                                                        // Friction coefficient — only for lateral view, top-down uses updateFriction() method
    );
    

    /*****************
     *** RENDERING ***
     *****************/
    
    // Initialize GLFW and glad, and create window
    micrasverse::render::Screen screen;

    // Check if all went well with the window creation
    if (!screen.init()) {
        glfwTerminate();
        std::cerr << "Failed to create window" << '\n';
        return -1;
    }

    // Set OpenGL viewport size and callback functions
    screen.setParameters();

    // Create camera
    micrasverse::render::Camera camera;

    // Create shader
    // ** To-do: pass this to Model's .render() method
    micrasverse::render::Shader flatColorShader("./render/assets/vertex-core.glsl", "./render/assets/fragment-core.glsl");
    
    // Create render models for maze and Micras
    // ** To-do: create Maze and MicrasModel classes that inherits from Model
    // ** To-do: pass these to Maze and MicrasBody classes and create a .render() method 
    micrasverse::render::Rectangle realMaze(
        glm::vec3(micrasverse::MAZE_FLOOR_HALFWIDTH, micrasverse::MAZE_FLOOR_HALFHEIGHT, 0.0f), // Position of the center
        glm::vec3(micrasverse::MAZE_FLOOR_WIDTH, micrasverse::MAZE_FLOOR_HEIGHT, 0.01f),        // Size
        glm::vec3(15.0f, 15.0f, 15.0f) / 255.0f                                                 // Color (RGB from 0 to 1)
    );
    micrasverse::render::Rectangle micrasRender(
        glm::vec3(micrasBody.getPosition().x , micrasBody.getPosition().y, 0.0f),               // Position of the center
        glm::vec3(micrasverse::MICRAS_WIDTH, micrasverse::MICRAS_HEIGHT, 0.01f),                // Size
        glm::vec3(15.0f, 125.0f, 15.0f) / 255.0f                                                // Color (RGB from 0 to 1)
    );
    
    // Create render models for maze walls and lattice points
    // ** To-do: pass this to Maze class .render() method
    std::vector<micrasverse::physics::Maze::Element> mazeElements = maze.getElements();
    std::vector<micrasverse::render::Rectangle> mazeWalls;
    mazeWalls.reserve(mazeElements.size());
    for (auto& element : mazeElements) {
        micrasverse::render::Rectangle newWall(
            glm::vec3(element.position.x, element.position.y, 0.0f),                            // Position of the center
            glm::vec3(element.size.x, element.size.y, 0.01f),                                   // Size
            glm::vec3(125.0f, 15.0f, 15.0f) / 255.0f                                            // Color (RGB from 0 to 1)
        );
        mazeWalls.push_back(newWall);
    }

    // Initialize render models
    // ** To-do: pass this to the class constructor or where possible
    realMaze.init();
    micrasRender.init();
    for (auto& wall : mazeWalls) {
        wall.init();
    }

    /*************
     *** IMGUI ***
     *************/

    // ** To-do: pass this to a GUI class
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(screen.getWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330");
    

    /*****************
     *** MAIN LOOP ***
     *****************/
    bool isRunning = false;
    bool achievedGoal = false;
    bool followMicras = true;
    bool plotDistanceSensorReadings = false;

    static std::vector<float> xValues, yValues;

    // utility structure for realtime plot
    // ** To-do: pass this to a plot class
    struct ScrollingBuffer {
        int MaxSize;
        int Offset;
        ImVector<ImVec2> Data;
        ScrollingBuffer(int max_size = 2000) {
            MaxSize = max_size;
            Offset  = 0;
            Data.reserve(MaxSize);
        }
        void AddPoint(float x, float y) {
            if (Data.size() < MaxSize)
                Data.push_back(ImVec2(x,y));
            else {
                Data[Offset] = ImVec2(x,y);
                Offset =  (Offset + 1) % MaxSize;
            }
        }
        void Erase() {
            if (Data.size() > 0) {
                Data.shrink(0);
                Offset  = 0;
            }
        }
    };

    // utility structure for realtime plot
    // ** To-do: pass this to a plot class
    struct RollingBuffer {
        float Span;
        ImVector<ImVec2> Data;
        RollingBuffer() {
            Span = 10.0f;
            Data.reserve(2000);
        }
        void AddPoint(float x, float y) {
            float xmod = fmodf(x, Span);
            if (!Data.empty() && xmod < Data.back().x)
                Data.shrink(0);
            Data.push_back(ImVec2(xmod, y));
        }
    };

    b2Vec2 error = {0.0, 0.0};
    b2Vec2 lastError = error;

    b2Vec2 goal = {0.09f + 0.012f, 0.09f + 0.012f + 0.0f};

    // ** To-do: pass this to a .init() method in the MicrasBody class
    micrasBody.distanceSensors.reserve(4);
    micrasBody.attachDistanceSensor({ 0.028f, 0.045f},  B2_PI / 2.0f);
    micrasBody.attachDistanceSensor({-0.028f, 0.045f},  B2_PI / 2.0f);
    micrasBody.attachDistanceSensor({ 0.009f,  0.055f}, B2_PI / 6.0f);
    micrasBody.attachDistanceSensor({-0.009f,  0.055f}, 5.0f * B2_PI / 6.0f);

    // ** To-do: pass this to a .init() method in the MicrasBody class
    micrasBody.motors.reserve(2);
    micrasBody.attachMotor({micrasverse::MICRAS_HALFWIDTH, 0.0f}, false);
    micrasBody.attachMotor({-micrasverse::MICRAS_HALFWIDTH,  0.0f}, true);

    // ** To-do: pass this to DistanceSensor class
    for (auto& sensor : micrasBody.distanceSensors) {
        sensor.rayRender.init();
    }

    while (!screen.shouldClose()) {
        
        // Calculate delta time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // Process keyboard and mouse inputs for camera
        camera.processInput(deltaTime);

        // Run simulation after pressing 'R'
        if (micrasverse::render::Keyboard::keyWentDown(GLFW_KEY_R)) {
            isRunning = !isRunning;
        }

        // Run simulation
        if (isRunning) {
            world.runStep(1.0f/120.0f, 8);
            
            // ** To-do: pass these to a .update() method in the MicrasBody class
            for (auto& sensor : micrasBody.distanceSensors) {
                sensor.getReading();
            }
            micrasBody.processInput(deltaTime);
            for (auto& motor : micrasBody.motors) {
                motor.update();
            }
            micrasBody.updateFriction();
        }

        // Update camera position to follow Micras
        // ** To-do: pass this to the Camera class
        if (micrasverse::render::Keyboard::keyWentDown(GLFW_KEY_F)) {
            followMicras = !followMicras;
        }
        if (followMicras) {
            camera.position = {micrasBody.getPosition().x, micrasBody.getPosition().y, camera.position.z};
        }

        // Center the camera on the maze when 'C' is pressed
        // ** To-do: pass this to the Camera class
        if (micrasverse::render::Keyboard::keyWentDown(GLFW_KEY_C)) {
            followMicras = false;
            camera.position = {micrasverse::MAZE_FLOOR_HALFWIDTH, micrasverse::MAZE_FLOOR_HALFHEIGHT, camera.position.z};
        }
        
        // Update position of micrasRender to match micrasBody
        // ** To-do: pass this to a .update() method in the MicrasBody class
         micrasRender.setPose(glm::vec3(micrasBody.getPosition().x, micrasBody.getPosition().y, micrasRender.position.z), micrasBody.getRotation());

        // Clear the screen color buffer
        screen.update();
        // ** To-do: pass these to screen class through a GUI class
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Create view matrix
        glm::mat4 view = camera.getViewMatrix();
        
        // Create projection matrix
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(camera.getZoom()), (float)micrasverse::render::Screen::SCR_WIDTH / (float)micrasverse::render::Screen::SCR_HEIGHT, 0.1f, 100.0f);

        // Activate shader
        flatColorShader.activate(view, projection);
        
        // Render maze, maze walls and Micras (order defines the z-index if position in z-axis is the same)
        // ** To-do: pass this to a .render() method in each class
        realMaze.render(flatColorShader, true);
        micrasRender.render(flatColorShader, false); // setModel = false -> makes model matrix able to be updated with object pose
        for (auto& wall : mazeWalls) {
            wall.render(flatColorShader, true);
        }
        // Render distance sensors rays
        for (auto& sensor : micrasBody.distanceSensors) {
            sensor.rayRender.render(flatColorShader, false);
        }

        // Render IMGUI
        // ** To-do: pass this to a GUI class (probably inside screen class)
        ImGui::Begin("Micrasverse");
        ImGui::Text("Press 'R' to run simulation");
        ImGui::Text("Press 'F' to follow Micras");
        ImGui::Text("Press 'C' to center camera on maze");
        ImGui::Text("Press 'ESC' to exit");
        ImGui::Text("Micras position: (%.2f, %.2f)", micrasBody.getPosition().x, micrasBody.getPosition().y);
        ImGui::Text("Micras velocity: (%.2f, %.2f)", b2Body_GetLinearVelocity(micrasBody.getBodyId()).x, b2Body_GetLinearVelocity(micrasBody.getBodyId()).y);
        ImGui::Text("Micras angular velocity: %.2f", b2Body_GetAngularVelocity(micrasBody.getBodyId()));
        ImGui::Text("Micras angle: %.2f", micrasBody.getRotation());
        ImGui::Text("Micras goal: (%.2f, %.2f)", goal.x, goal.y);

        // ** To-do: pass this to a plot class
        if (micrasverse::render::Keyboard::keyWentDown(GLFW_KEY_P)) {
            plotDistanceSensorReadings = !plotDistanceSensorReadings;
        }

        if(plotDistanceSensorReadings) {
            ImGui::BulletText("Distance sensors data. Front sensors in first plot. Left and right sensors in second plot.");
            //ImGui::BulletText("This example assumes 60 FPS. Higher FPS requires larger buffer size.");
            static ScrollingBuffer sdata1, sdata2;
            static RollingBuffer   rdata1, rdata2;
            static float t = 0;
            t += ImGui::GetIO().DeltaTime;
            sdata1.AddPoint(t, micrasBody.motors[0].getCurrent());
            rdata1.AddPoint(t, micrasBody.distanceSensors[2].getReading());
            sdata2.AddPoint(t, micrasBody.motors[1].getCurrent());
            rdata2.AddPoint(t, micrasBody.distanceSensors[3].getReading());

            static float history = 10.0f;
            ImGui::SliderFloat("History",&history,1,30,"%.1f s");
            rdata1.Span = history;
            rdata2.Span = history;

            static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;

            if (ImPlot::BeginPlot("##Scrolling", ImVec2(-1,150))) {
                ImPlot::SetupAxes(nullptr, nullptr, flags, flags);
                ImPlot::SetupAxisLimits(ImAxis_X1,t - history, t, ImGuiCond_Always);
                ImPlot::SetupAxisLimits(ImAxis_Y1,-0.2f,+0.2f);
                ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL,0.5f);
                ImPlot::PlotShaded("Current motor 0", &sdata1.Data[0].x, &sdata1.Data[0].y, sdata1.Data.size(), -INFINITY, 0, sdata1.Offset, 2 * sizeof(float));
                ImPlot::PlotLine("Current motor 1", &sdata2.Data[0].x, &sdata2.Data[0].y, sdata2.Data.size(), 0, sdata2.Offset, 2*sizeof(float));
                ImPlot::EndPlot();
            }
            if (ImPlot::BeginPlot("##Rolling", ImVec2(-1,150))) {
                ImPlot::SetupAxes(nullptr, nullptr, flags, flags);
                ImPlot::SetupAxisLimits(ImAxis_X1,0,history, ImGuiCond_Always);
                ImPlot::SetupAxisLimits(ImAxis_Y1,0,1);
                ImPlot::PlotLine("Distance sensor 2", &rdata1.Data[0].x, &rdata1.Data[0].y, rdata1.Data.size(), 0, 0, 2 * sizeof(float));
                ImPlot::PlotLine("Distance sensor 3", &rdata2.Data[0].x, &rdata2.Data[0].y, rdata2.Data.size(), 0, 0, 2 * sizeof(float));
                ImPlot::EndPlot();
            }       
        }

        // ** To-do: pass this to a GUI class (probably inside screen class)
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        //Swap front (screen) buffer and back (rendering) buffer + poll events like window resize, key and mouse input etc.
        screen.newFrame();        
    }

    // Delete VAO, VBO and EBO buffers
    // ** To-do: check if this can be done in the destructor
    realMaze.cleanUp();
    micrasRender.cleanUp();
    for (auto& wall : mazeWalls) {
        wall.cleanUp();
    }
    for (auto& sensor : micrasBody.distanceSensors) {
        sensor.rayRender.cleanUp();
    }

    // Shutdown IMGUI
    // ** To-do: pass this to a GUI class (probably inside screen class)
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
    
    // Terminate GLFW
    glfwTerminate();

    return 0;

} //end of main
