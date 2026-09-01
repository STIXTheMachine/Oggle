cmake_minimum_required(VERSION 3.15)

option(OGGLE_USE_OPENGL "Use OpenGL Backend" ON)
option(OGGLE_USE_VULKAN "Use Vulkan Backend" OFF)
option(OGGLE_USE_D3D12  "Use D3D12 Backend"  OFF)

if(
        OGGLE_USE_OPENGL AND (OGGLE_USE_VULKAN OR OGGLE_USE_D3D12) OR
        OGGLE_USE_VULKAN AND (OGGLE_USE_OPENGL OR OGGLE_USE_D3D12) OR
        OGGLE_USE_D3D12  AND (OGGLE_USE_OPENGL OR OGGLE_USE_VULKAN)
)
    message(FATAL_ERROR "Exactly one graphics backend must be enabled. (OpenGL, Vulkan, or D3D12)")
endif()

if(OGGLE_USE_OPENGL)
endif()

if(OGGLE_USE_VULKAN)
    message(FATAL_ERROR "Oggle does not yet support Vulkan.")
endif()

if(OGGLE_USE_D3D12)
    message(FATAL_ERROR "Oggle does not yet support D3D12.")
endif()

set(CMAKE_CXX_STANDARD 23)

set(TARGET ${PROJECT_NAME})
set(OGGLE_THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/ThirdParty)

# Framework Modules
add_subdirectory(Source/Core)
add_subdirectory(Source/Windowing)
add_subdirectory(Source/Renderer)
add_subdirectory(Source/Application)

# Applications
add_library(Oggle INTERFACE
        ../TestSuite/Source/Private/main.cpp)

target_link_libraries(Oggle INTERFACE Oggle::Core)
target_link_libraries(Oggle INTERFACE Oggle::Windowing)
target_link_libraries(Oggle INTERFACE Oggle::Application)
target_link_libraries(Oggle INTERFACE Oggle::Renderer)