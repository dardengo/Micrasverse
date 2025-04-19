# Set C++ standard for this module
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Source files
file(GLOB_RECURSE PROXY_SOURCES "src/*.cpp")
file(GLOB_RECURSE PROXY_HEADERS "include/micras/*.hpp")

# Create library target
add_library(proxy_module STATIC ${PROXY_SOURCES} ${PROXY_HEADERS})

# Set include directories
target_include_directories(proxy_module PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/core/include
    ${CMAKE_SOURCE_DIR}/src/physics/include
    ${CMAKE_SOURCE_DIR}/external/MicrasFirmware/micras_nav/include
    ${CMAKE_SOURCE_DIR}/external/MicrasFirmware/micras_core/include
    ${CMAKE_SOURCE_DIR}/external/MicrasFirmware/include
    ${CMAKE_SOURCE_DIR}/external/MicrasFirmware/src
    ${CMAKE_SOURCE_DIR}/external/imgui
    ${CMAKE_SOURCE_DIR}/external/imgui/backends
)

# Link dependencies
target_link_libraries(proxy_module PUBLIC
    physics_engine
    micras_nav
    micras_core
    imgui
    OpenGL::GL
)

# Create alias target
add_library(micras::proxy ALIAS proxy_module) 
