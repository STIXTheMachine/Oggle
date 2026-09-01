cmake_minimum_required(VERSION 3.15)
set(MODULE Testing)

add_subdirectory(ThirdParty/doctest-2.5.3)
add_library(${MODULE} INTERFACE)
target_link_libraries(${MODULE} INTERFACE doctest)

add_library(Oggle::${MODULE} ALIAS ${MODULE})