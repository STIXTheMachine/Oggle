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
)

# Make MSVC behave itself. The fact that the standard compliant preprocessor isn't the default is fucking wild.
if(MSVC)
    target_compile_options(Oggle PRIVATE /Zc:preprocessor)
endif()

add_custom_target(COPY_SHADERS ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/Shaders/
        ${CMAKE_BINARY_DIR}/Assets/Shaders
)

add_dependencies(${PROJECT_NAME} COPY_SHADERS)

if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebugInfo")
    target_compile_definitions(${TARGET} PRIVATE BUILD_DEBUG=1)
else()
    target_compile_definitions(${TARGET} PRIVATE BUILD_DEBUG=0)
endif()

# OpenGL
find_package(OpenGL REQUIRED)

# GLFW
set(GLFW_DIR "ThirdParty/Vendor/glfw-3.4")
add_subdirectory(${GLFW_DIR})
target_include_directories(${TARGET} PRIVATE "ThirdParty/Vendor/glfw-3.4/include")

# GLBinding
set(GLBINDING_DIR "ThirdParty/Vendor/glbinding-3.5.0")
add_subdirectory(${GLBINDING_DIR})
target_include_directories(${TARGET} PRIVATE "ThirdParty/Vendor/glbinding-3.5.0/source/glbinding/include")
target_include_directories(${TARGET} PRIVATE "ThirdParty/Vendor/glbinding-3.5.0/source/glbinding-aux/include")

include(FetchContent)

# GLM
FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 	   8d1fd52e5ab5590e2c81768ace50c72bae28f2ed # 1.0.3
)
FetchContent_MakeAvailable(glm)


target_include_directories(${TARGET} PRIVATE "include")
target_include_directories(${TARGET} PRIVATE "ThirdParty/Headers")
target_link_libraries(${TARGET} PRIVATE
        glfw
        OpenGL::GL
        glbinding::glbinding
        glbinding::glbinding-aux
        glm::glm
)


# ===========================================================================================
# ========================================== Tests ==========================================
# ===========================================================================================
set(TARGET Tests)

add_executable(${TARGET}
        tests/test_main.cpp
        tests/Core/TypeTraitTests.cpp
)

# OpenGL
find_package(OpenGL REQUIRED)

# GLFW
target_include_directories(${TARGET} PRIVATE "ThirdParty/Vendor/glfw-3.4/include")

# GLBinding
target_include_directories(${TARGET} PRIVATE "ThirdParty/Vendor/glbinding-3.5.0/source/glbinding/include")
target_include_directories(${TARGET} PRIVATE "ThirdParty/Vendor/glbinding-3.5.0/source/glbinding-aux/include")

target_include_directories(${TARGET} PRIVATE "include")
target_include_directories(${TARGET} PRIVATE "ThirdParty/Headers")
target_link_libraries(${TARGET} PRIVATE
        glfw
        OpenGL::GL
        glbinding::glbinding
        glbinding::glbinding-aux
)
