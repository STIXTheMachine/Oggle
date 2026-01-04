cmake_minimum_required(VERSION 3.15)
set(MODULE Windowing)

file(
        GLOB_RECURSE
        SOURCES
        CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

file(
        GLOB
        IMGUI_SOURCES
        "${OGGLE_THIRD_PARTY_DIR}/imgui-1.92.5-docking/*.cpp"
        "${OGGLE_THIRD_PARTY_DIR}/imgui-1.92.5-docking/*.h"
)

file(
        GLOB
        IMGUI_IMPLEMENTATIONS
        "${OGGLE_THIRD_PARTY_DIR}/imgui-1.92.5-docking/backends/imgui_impl_glfw.h"
        "${OGGLE_THIRD_PARTY_DIR}/imgui-1.92.5-docking/backends/imgui_impl_glfw.cpp"
        "${OGGLE_THIRD_PARTY_DIR}/imgui-1.92.5-docking/backends/imgui_impl_opengl3.h"
        "${OGGLE_THIRD_PARTY_DIR}/imgui-1.92.5-docking/backends/imgui_impl_opengl3.cpp"
)

add_library(${MODULE} STATIC ${SOURCES} ${IMGUI_SOURCES} ${IMGUI_IMPLEMENTATIONS})

target_include_directories(
        ${MODULE}
        PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${OGGLE_THIRD_PARTY_DIR}/imgui-1.92.5-docking"
        "${OGGLE_THIRD_PARTY_DIR}/imgui-1.92.5-docking/backends"
)

target_link_libraries(${MODULE} PUBLIC Oggle::Core)
target_link_libraries(${MODULE} PRIVATE glfw)
target_link_libraries(${MODULE} PRIVATE glbinding::glbinding)

add_library(Oggle::${MODULE} ALIAS ${MODULE})