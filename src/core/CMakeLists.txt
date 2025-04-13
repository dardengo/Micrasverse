file(GLOB_RECURSE CORE_SOURCES "src/*.cpp")
file(GLOB_RECURSE CORE_HEADERS "include/*.hpp")

# If no source files are found, create an empty one
if("${CORE_SOURCES}" STREQUAL "")
  set(CORE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/empty.cpp")
  file(WRITE "${CMAKE_CURRENT_SOURCE_DIR}/empty.cpp" "// Empty file to satisfy CMake\n")
endif()

add_library(micrasverse_core STATIC ${CORE_SOURCES} ${CORE_HEADERS})

target_include_directories(micrasverse_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(micrasverse_core PUBLIC
    config_module
) 
