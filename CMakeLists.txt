cmake_minimum_required(VERSION 3.15)
project(Oggle)

set(CMAKE_CXX_STANDARD 23)
set(TARGET ${PROJECT_NAME})

add_executable(${TARGET} src/main.cpp)

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

target_include_directories(${TARGET} PRIVATE "include")
target_include_directories(${TARGET} PRIVATE "ThirdParty/Headers")
target_link_libraries(${TARGET} PRIVATE
        glfw
        OpenGL::GL
        glbinding::glbinding
        glbinding::glbinding-aux
)

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