cmake_minimum_required(VERSION 3.15)
project(OggleEngine)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(FATAL_ERROR "OggleEngline only supports GCC. Please configure your project to use GCC.")
endif()

add_subdirectory(Engine)
add_subdirectory(Projects/OggleApp)
add_subdirectory(Projects/Scratch)
add_subdirectory(TestSuite)