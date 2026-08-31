cmake_minimum_required(VERSION 3.15)
set(MODULE Renderer)

add_library(${MODULE} STATIC
        Private/Renderer.cpp
        Private/RenderingModule.cpp
        Private/LogRenderer.cpp
)

target_include_directories(
        ${MODULE}
        PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/Public"
        "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty"
)

add_subdirectory(ThirdParty/OpenGL)

target_link_libraries(${MODULE} PUBLIC Oggle::Core)
target_link_libraries(${MODULE} PUBLIC Oggle::Windowing)
target_link_libraries(${MODULE} PUBLIC glad)

add_library(Oggle::${MODULE} ALIAS ${MODULE})

target_compile_definitions(
        ${MODULE} PRIVATE
        $<$<BOOL:${OGGLE_USE_OPENGL}>:OGGLE_GRAPHICS_API_OPENGL>
        $<$<BOOL:${OGGLE_USE_VULKAN}>:OGGLE_GRAPHICS_API_VULKAN>
        $<$<BOOL:${OGGLE_USE_D3D12}>:OGGLE_GRAPHICS_API_D3D12>
)