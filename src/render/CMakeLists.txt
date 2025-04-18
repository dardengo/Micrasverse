file(GLOB_RECURSE RENDER_SOURCES "*.cpp")
file(GLOB_RECURSE RENDER_HEADERS "include/*.hpp")

# Get GUI sources separately
file(GLOB_RECURSE GUI_SOURCES "GUI/*.cpp")
file(GLOB_RECURSE GUI_HEADERS "GUI/include/*.hpp")

add_library(render_engine STATIC 
    ${RENDER_SOURCES} 
    ${RENDER_HEADERS}
    ${GUI_SOURCES}
    ${GUI_HEADERS}
)

target_include_directories(render_engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/GUI/include
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/glm
    ${CMAKE_SOURCE_DIR}/src/simulation/include
    ${CMAKE_SOURCE_DIR}/src/physics/include
    ${CMAKE_SOURCE_DIR}/src/config
    ${CMAKE_SOURCE_DIR}/src/core/include
    ${CMAKE_SOURCE_DIR}/src/render/include
    ${CMAKE_SOURCE_DIR}/external/box2d/include
)

# Find required packages
# find_package(OpenGL REQUIRED)

target_link_libraries(render_engine PUBLIC
    glad
    imgui
    implot
    glfw
    OpenGL::GL
    io_module
    physics_engine
    config_module
    simulation_engine
) 
