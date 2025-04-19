# External dependencies

# Box2D
if(NOT TARGET box2d)
  add_subdirectory(box2d)
endif()

# GLM
if(NOT TARGET glm)
  add_subdirectory(glm)
endif()

# ImGui
if(NOT TARGET imgui)
  set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
  file(GLOB IMGUI_SOURCES ${IMGUI_DIR}/*.cpp)
  file(GLOB IMGUI_HEADERS ${IMGUI_DIR}/*.h)
  
  # ImGui backends
  list(APPEND IMGUI_SOURCES
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
  )
  list(APPEND IMGUI_HEADERS
    ${IMGUI_DIR}/backends/imgui_impl_glfw.h
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.h
  )
  
  add_library(imgui STATIC ${IMGUI_SOURCES} ${IMGUI_HEADERS})
  target_include_directories(imgui PUBLIC 
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
  )
  
  # GLFW is required for ImGui
  find_package(glfw3 REQUIRED)
  target_link_libraries(imgui PUBLIC glfw)
endif()

# ImPlot
if(NOT TARGET implot)
  set(IMPLOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/implot)
  file(GLOB IMPLOT_SOURCES ${IMPLOT_DIR}/*.cpp)
  file(GLOB IMPLOT_HEADERS ${IMPLOT_DIR}/*.h)
  
  add_library(implot STATIC ${IMPLOT_SOURCES} ${IMPLOT_HEADERS})
  target_include_directories(implot PUBLIC ${IMPLOT_DIR})
  target_link_libraries(implot PUBLIC imgui)
endif() 

# MicrasFirmware - only include necessary components
if(NOT TARGET micras_nav)
  add_subdirectory(MicrasFirmware/micras_core)
  add_subdirectory(MicrasFirmware/micras_nav)
  
  # Create an interface target for MicrasFirmware that excludes config directory
  add_library(micras STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/MicrasFirmware/src/micras.cpp
  )
  target_link_libraries(micras PUBLIC micras::core micras::nav)
  target_include_directories(micras PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/MicrasFirmware/include
    ${CMAKE_CURRENT_SOURCE_DIR}/MicrasFirmware/micras_core/include
    ${CMAKE_CURRENT_SOURCE_DIR}/MicrasFirmware/micras_nav/include
  )
endif() 
