file(GLOB_RECURSE CORE_SOURCES "*.cpp")
file(GLOB_RECURSE CORE_HEADERS "include/*.hpp")

add_library(core_module STATIC ${CORE_SOURCES} ${CORE_HEADERS})

target_include_directories(core_module PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include
) 
