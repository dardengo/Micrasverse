file(GLOB_RECURSE PHYSICS_SOURCES "*.cpp")
file(GLOB_RECURSE PHYSICS_HEADERS "include/*.hpp")

add_library(physics_engine STATIC ${PHYSICS_SOURCES} ${PHYSICS_HEADERS})

target_include_directories(physics_engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/box2d/include
    ${CMAKE_SOURCE_DIR}/external/glm
    ${CMAKE_SOURCE_DIR}/src/config/include
    ${CMAKE_SOURCE_DIR}/src/core/include
    ${CMAKE_SOURCE_DIR}/src/proxy/include
    ${CMAKE_SOURCE_DIR}/src/io/include
)

target_link_libraries(physics_engine PUBLIC
    box2d
    core_module
    config_module
    proxy_module
    io_module
) 
