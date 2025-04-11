file(GLOB_RECURSE SIMULATION_SOURCES "*.cpp")
file(GLOB_RECURSE SIMULATION_HEADERS "include/*.hpp")

add_library(simulation_engine STATIC ${SIMULATION_SOURCES} ${SIMULATION_HEADERS})

target_include_directories(simulation_engine PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/render/include
)

target_link_libraries(simulation_engine PUBLIC
    physics_engine
) 
