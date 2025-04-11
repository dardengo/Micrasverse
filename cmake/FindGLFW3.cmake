# Locate the GLFW library
#
# This module defines the following variables:
#
# GLFW3_FOUND        - true if GLFW3 was found
# GLFW3_INCLUDE_DIRS - include directories for GLFW3
# GLFW3_LIBRARIES    - libraries to link against GLFW3
#
# It also defines the following imported targets:
#
# glfw    - The GLFW3 library

find_path(GLFW3_INCLUDE_DIR
  NAMES GLFW/glfw3.h
  PATHS
    ${CMAKE_SOURCE_DIR}/external/box2d/build/_deps/glfw-src/include
    /usr/include
    /usr/local/include
    /opt/local/include
    /sw/include
  PATH_SUFFIXES
    GLFW
)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(GLFW_LIB_SUFFIX "64")
else()
  set(GLFW_LIB_SUFFIX "")
endif()

if(WIN32)
  find_library(GLFW3_LIBRARY
    NAMES glfw3 glfw3dll glfw
    PATHS
      ${CMAKE_SOURCE_DIR}/lib
      ${CMAKE_SOURCE_DIR}/external/box2d/build/_deps/glfw-build/src
      $ENV{GLFW_ROOT}/lib
      /usr/lib
      /usr/local/lib
      /opt/local/lib
      /sw/lib
  )
elseif(APPLE)
  find_library(GLFW3_LIBRARY
    NAMES glfw glfw3
    PATHS
      ${CMAKE_SOURCE_DIR}/lib
      $ENV{GLFW_ROOT}/lib
      /usr/lib
      /usr/local/lib
      /opt/local/lib
      /sw/lib
  )
else()
  find_library(GLFW3_LIBRARY
    NAMES glfw glfw3
    PATHS
      ${CMAKE_SOURCE_DIR}/lib
      $ENV{GLFW_ROOT}/lib
      /usr/lib
      /usr/local/lib
      /opt/local/lib
      /sw/lib
      /usr/lib/x86_64-linux-gnu
  )
endif()

set(GLFW3_INCLUDE_DIRS ${GLFW3_INCLUDE_DIR})
set(GLFW3_LIBRARIES ${GLFW3_LIBRARY})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLFW3 DEFAULT_MSG GLFW3_LIBRARY GLFW3_INCLUDE_DIR)

mark_as_advanced(GLFW3_INCLUDE_DIR GLFW3_LIBRARY)

if(GLFW3_FOUND AND NOT TARGET glfw)
  add_library(glfw UNKNOWN IMPORTED)
  set_target_properties(glfw PROPERTIES
    IMPORTED_LOCATION "${GLFW3_LIBRARIES}"
    INTERFACE_INCLUDE_DIRECTORIES "${GLFW3_INCLUDE_DIRS}"
  )
endif() 
