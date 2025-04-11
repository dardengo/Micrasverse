file(GLOB_RECURSE CONFIG_SOURCES "*.cpp")
file(GLOB_RECURSE CONFIG_HEADERS "include/*.hpp")

add_library(config_module STATIC ${CONFIG_SOURCES} ${CONFIG_HEADERS})

target_include_directories(config_module PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include
) 
