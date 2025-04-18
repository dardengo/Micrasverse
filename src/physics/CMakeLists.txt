# Set C++ standard for this module
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Source files
file(GLOB_RECURSE PHYSICS_SOURCES "*.cpp")
file(GLOB_RECURSE PHYSICS_HEADERS "include/*.hpp")

# Create library target
add_library(physics_engine STATIC ${PHYSICS_SOURCES} ${PHYSICS_HEADERS})

# Set include directories
target_include_directories(physics_engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/box2d/include
    ${CMAKE_SOURCE_DIR}/external/glm
    ${CMAKE_SOURCE_DIR}/src/config
    ${CMAKE_SOURCE_DIR}/src/core/include
    ${CMAKE_SOURCE_DIR}/src/io/include
    ${CMAKE_SOURCE_DIR}/src/proxy/include
    ${CMAKE_SOURCE_DIR}/external/MicrasFirmware
)

# Link dependencies
target_link_libraries(physics_engine PUBLIC
    box2d
    micrasverse_core
    config_module
    io_module
)

# Create alias target
add_library(micras::physics ALIAS physics_engine) 
