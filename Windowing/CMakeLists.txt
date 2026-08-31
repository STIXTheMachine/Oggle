cmake_minimum_required(VERSION 3.15)
set(MODULE Windowing)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(ThirdParty/glfw-3.4)

add_library(${MODULE} STATIC
        Private/LogWindowing.cpp
        Private/SpawnTestWindow.cpp
        Private/Window.cpp
        Private/WindowingModule.cpp
)

target_include_directories(${MODULE} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Public")

target_link_libraries(${MODULE} PUBLIC Oggle::Core)
target_link_libraries(${MODULE} PUBLIC glfw)

add_library(Oggle::${MODULE} ALIAS ${MODULE})