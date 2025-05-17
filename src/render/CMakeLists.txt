# List render sources explicitly (excluding model.cpp which was converted to header-only)

file(GLOB_RECURSE RENDER_HEADERS "include/*.hpp")

# Get GUI sources separately
#file(GLOB_RECURSE GUI_SOURCES "GUI/*.cpp")
#file(GLOB_RECURSE GUI_HEADERS "GUI/include/*.hpp")

file(GLOB_RECURSE LVE_SOURCES "vulkan_engine/*.cpp")
file(GLOB_RECURSE LVE_HEADERS "vulkan_engine/include/*.hpp")

# Find Vulkan package
find_package(Vulkan REQUIRED)

add_library(render_engine STATIC 
    ${RENDER_SOURCES} 
    ${RENDER_HEADERS}
#    ${GUI_SOURCES}
#    ${GUI_HEADERS}
    ${LVE_SOURCES}
    ${LVE_HEADERS}
)

# Create shader directory in binary output
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/shaders)

# Copy pre-compiled SPIR-V shaders directly
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders/vert.spv
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/shaders/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders/frag.spv
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/shaders/)

target_include_directories(render_engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/GUI/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vulkan_engine/include
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/glm
    ${CMAKE_SOURCE_DIR}/src/simulation/include
    ${CMAKE_SOURCE_DIR}/src/physics/include
    ${CMAKE_SOURCE_DIR}/src/config
    ${CMAKE_SOURCE_DIR}/src/core/include
    ${CMAKE_SOURCE_DIR}/src/render/include
    ${CMAKE_SOURCE_DIR}/external/box2d/include
    ${CMAKE_SOURCE_DIR}/external/MicrasFirmware/micras_nav/include
    ${CMAKE_SOURCE_DIR}/external/MicrasFirmware/micras_core/include
    ${CMAKE_SOURCE_DIR}/external/MicrasFirmware/include
    ${CMAKE_SOURCE_DIR}/external/MicrasFirmware
    ${CMAKE_SOURCE_DIR}/external
    ${CMAKE_SOURCE_DIR}/external/MicrasFirmware/src
    ${Vulkan_INCLUDE_DIRS}
)

# Add Vulkan definition for GLFW
target_compile_definitions(render_engine PRIVATE
    GLFW_INCLUDE_VULKAN
)

# Copy shaders to build directory
add_custom_command(TARGET render_engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:render_engine>/shaders
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/vert.spv
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/frag.spv
        $<TARGET_FILE_DIR:render_engine>/shaders/
)

target_link_libraries(render_engine PUBLIC
    imgui
    implot
    implot3d
    glfw
    ${Vulkan_LIBRARIES}
    io_module
    physics_engine
    config_module
    simulation_engine
) 
