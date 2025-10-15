cmake_minimum_required(VERSION 3.15)
project(Oggle)

set(CMAKE_CXX_STANDARD 23)

add_executable(${PROJECT_NAME} main.cpp)

# OpenGL
find_package(OpenGL REQUIRED)

# GLFW
set(GLFW_DIR "dep/glfw-3.4")
add_subdirectory(${GLFW_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE "dep/glfw-3.4/include")

# GLBinding
set(GLBINDING_DIR "dep/glbinding-3.5.0")
add_subdirectory(${GLBINDING_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE "dep/glbinding-3.5.0/source/glbinding/include")
target_include_directories(${PROJECT_NAME} PRIVATE "dep/glbinding-3.5.0/source/glbinding-aux/include")

target_link_libraries(${PROJECT_NAME} PRIVATE
        glfw
        OpenGL::GL
        glbinding::glbinding
        glbinding::glbinding-aux
)
