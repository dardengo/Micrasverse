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
) 
