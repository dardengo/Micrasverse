file(GLOB_RECURSE PROXY_SOURCES "src/*.cpp")
file(GLOB_RECURSE PROXY_HEADERS "include/*.hpp")

add_library(proxy_module STATIC ${PROXY_SOURCES} ${PROXY_HEADERS})

target_include_directories(proxy_module PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(proxy_module PUBLIC
    physics_engine
) 
