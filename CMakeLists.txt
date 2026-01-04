cmake_minimum_required(VERSION 3.15)
project(Oggle)

set(CMAKE_CXX_STANDARD 23)
set(TARGET ${PROJECT_NAME})
set(OGGLE_THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/ThirdParty)

# Make MSVC behave itself. The fact that the standard compliant preprocessor isn't the default is fucking wild.
if(MSVC)
    target_compile_options(Oggle PRIVATE /Zc:preprocessor)
endif()

add_subdirectory(Core)
add_subdirectory(Windowing)
add_subdirectory(Renderer)
add_subdirectory(ThirdParty)
add_subdirectory(App/Scratch)
add_subdirectory(App/OggleApp)