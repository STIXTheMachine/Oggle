cmake_minimum_required(VERSION 4.0)
project(Oggle)

set(CMAKE_CXX_STANDARD 23)

# GLFW
set(GLFW_DIR "dep/glfw-3.4")
add_subdirectory(${GLFW_DIR})

add_executable(${PROJECT_NAME} main.cpp)