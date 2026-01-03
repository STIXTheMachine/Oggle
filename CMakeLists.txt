cmake_minimum_required(VERSION 3.15)
project(Oggle)

set(CMAKE_CXX_STANDARD 23)
set(TARGET ${PROJECT_NAME})

file(
        GLOB_RECURSE
        SOURCES
        CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_executable(
        ${TARGET}
        ${SOURCES}
        ThirdParty/Vendor/stb/src/stb_image.cpp
)

# Make MSVC behave itself. The fact that the standard compliant preprocessor isn't the default is fucking wild.
if(MSVC)
    target_compile_options(Oggle PRIVATE /Zc:preprocessor)
endif()

add_subdirectory(Core)
add_subdirectory(App/Scratch)