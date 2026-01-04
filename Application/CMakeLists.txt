cmake_minimum_required(VERSION 3.15)
set(MODULE Application)

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

target_link_libraries(${MODULE} PUBLIC Oggle::Core)
target_link_libraries(${MODULE} PUBLIC Oggle::Windowing)
target_link_libraries(${MODULE} PRIVATE Oggle::Renderer)
target_link_libraries(${MODULE} PRIVATE glfw)
target_link_libraries(${MODULE} PRIVATE glbinding::glbinding)

add_library(Oggle::${MODULE} ALIAS ${MODULE})
