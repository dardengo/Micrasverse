# Include GLAD
add_library(glad STATIC glad.c)
target_include_directories(glad PUBLIC 
    ${CMAKE_SOURCE_DIR}/external/box2d/build/_deps/glfw-src/deps
    ${CMAKE_SOURCE_DIR}/include
)

# Add subdirectories for all modules
add_subdirectory(config)
add_subdirectory(core)
add_subdirectory(io)
add_subdirectory(physics)
add_subdirectory(proxy)
add_subdirectory(render)
add_subdirectory(simulation)

# Main executable
add_executable(micrasverse main.cpp)

# Ensure main.cpp can be found
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
  message(FATAL_ERROR "main.cpp not found at ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
endif()

# Make dependencies explicit for clarity
target_link_libraries(micrasverse PRIVATE
    simulation_engine
    render_engine
    physics_engine
    io_module
    proxy_module
    config_module
    micrasverse_core
    micras
)

# Make sure it can find all needed headers
target_include_directories(micrasverse PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/config
    ${CMAKE_SOURCE_DIR}/src/core/include
    ${CMAKE_SOURCE_DIR}/src/io/include
    ${CMAKE_SOURCE_DIR}/src/physics/include
    ${CMAKE_SOURCE_DIR}/src/proxy/include
    ${CMAKE_SOURCE_DIR}/src/render/include
    ${CMAKE_SOURCE_DIR}/src/simulation/include
) 
