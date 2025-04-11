file(GLOB_RECURSE IO_SOURCES "*.cpp")
file(GLOB_RECURSE IO_HEADERS "include/*.hpp")

add_library(io_module STATIC ${IO_SOURCES} ${IO_HEADERS})

target_include_directories(io_module PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external
)

target_link_libraries(io_module PUBLIC
    glfw
    glm
) 
