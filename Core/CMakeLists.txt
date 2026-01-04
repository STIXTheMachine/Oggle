cmake_minimum_required(VERSION 3.15)
set(MODULE Core)

file(
        GLOB_RECURSE
        SOURCES
        CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_library(${MODULE} STATIC ${SOURCES})

target_include_directories(
        ${MODULE}
        PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

add_library(Oggle::${MODULE} ALIAS ${MODULE})