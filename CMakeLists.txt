cmake_minimum_required(VERSION 3.14)
project(Micrasverse VERSION 1.0 LANGUAGES C CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add custom CMake modules path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

# Default build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

# Output directories - handle multi-configuration generators (VS, Xcode)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Add these settings for multi-configuration generators (Visual Studio)
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/lib)
endforeach()

# Find required packages
find_package(OpenGL REQUIRED)
if(NOT TARGET OpenGL::GL)
    add_library(OpenGL::GL INTERFACE IMPORTED)
    target_link_libraries(OpenGL::GL INTERFACE ${OPENGL_LIBRARIES})
    target_include_directories(OpenGL::GL INTERFACE ${OPENGL_INCLUDE_DIR})
endif()

# Platform-specific settings
if(UNIX AND NOT APPLE)
  # Linux-specific settings
  set(LINUX TRUE)
  # Include directories for X11, OpenGL, etc.
  # find_package(X11 REQUIRED)
  # include_directories(${X11_INCLUDE_DIR})
endif()

# Options
option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

# Add subdirectories for modules
add_subdirectory(src)

# Handle external dependencies
add_subdirectory(external) 
