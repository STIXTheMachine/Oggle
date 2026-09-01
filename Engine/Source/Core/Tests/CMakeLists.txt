cmake_minimum_required(VERSION 3.15)
set(MODULE CoreTest)

add_library(${MODULE} STATIC Test.test.cpp)
target_link_libraries(${MODULE} PRIVATE Oggle::Testing)
target_link_libraries(${MODULE} PRIVATE Oggle::Core)

add_library(Oggle::${MODULE} ALIAS ${MODULE})