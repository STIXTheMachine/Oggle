cmake_minimum_required(VERSION 3.15)
set(MODULE Logging)

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
        PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include/${MODULE}"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/${MODULE}"
)

add_library(Oggle::${MODULE} ALIAS ${MODULE})
