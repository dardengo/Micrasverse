file(GLOB_RECURSE CONFIG_SOURCES "*.cpp")
file(GLOB_RECURSE CONFIG_HEADERS "include/*.hpp")

# If no source files are found, create an empty one
if("${CONFIG_SOURCES}" STREQUAL "")
  set(CONFIG_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/empty.cpp")
  file(WRITE "${CMAKE_CURRENT_SOURCE_DIR}/empty.cpp" "// Empty file to satisfy CMake\n")
endif()

add_library(config_module STATIC ${CONFIG_SOURCES} ${CONFIG_HEADERS})

target_include_directories(config_module PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/MicrasFirmware/micras_core/include
    ${CMAKE_SOURCE_DIR}/external/MicrasFirmware/micras_nav/include
    ${CMAKE_SOURCE_DIR}/src/proxy/include
    ${CMAKE_SOURCE_DIR}/external/box2d/include
    ${CMAKE_SOURCE_DIR}/src/core/include          # For micrasverse_core/types.hpp
    ${CMAKE_SOURCE_DIR}/src/physics/include       # For physics/argb.hpp and other physics headers
    ${CMAKE_SOURCE_DIR}/src/config                # For local constants.hpp
) 
