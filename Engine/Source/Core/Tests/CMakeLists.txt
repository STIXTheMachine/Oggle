cmake_minimum_required(VERSION 3.15)

add_library(CoreTests STATIC Test.test.cpp)
target_link_libraries(CoreTests PRIVATE Oggle::Core)