set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(OPTION_BUILD_TOOLS OFF CACHE BOOL "" FORCE)
set(OPTION_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(OPTION_USE_GIT_INFORMATION OFF CACHE BOOL "" FORCE)
add_subdirectory(glbinding-3.5.0)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw-3.4)

file(
        GLOB
        IMGUI_ROOT_SOURCES_GLOB
        "${OGGLE_THIRD_PARTY_DIR}/imgui-1.92.5-docking/*.cpp"
        "${OGGLE_THIRD_PARTY_DIR}/imgui-1.92.5-docking/*.h"
)

file(
        GLOB
        IMGUI_IMPLEMENTATIONS_GLOB
        "${OGGLE_THIRD_PARTY_DIR}/imgui-1.92.5-docking/backends/imgui_impl_glfw.h"
        "${OGGLE_THIRD_PARTY_DIR}/imgui-1.92.5-docking/backends/imgui_impl_glfw.cpp"
        "${OGGLE_THIRD_PARTY_DIR}/imgui-1.92.5-docking/backends/imgui_impl_opengl3.h"
        "${OGGLE_THIRD_PARTY_DIR}/imgui-1.92.5-docking/backends/imgui_impl_opengl3.cpp"
)

set(
        IMGUI_INCLUDES
        "${OGGLE_THIRD_PARTY_DIR}/imgui-1.92.5-docking"
        "${OGGLE_THIRD_PARTY_DIR}/imgui-1.92.5-docking/backends"
        PARENT_SCOPE
)

set(IMGUI_ROOT_SOURCES ${IMGUI_ROOT_SOURCES_GLOB} PARENT_SCOPE)
set(IMGUI_IMPLEMENTATIONS ${IMGUI_IMPLEMENTATIONS_GLOB} PARENT_SCOPE)
set(IMGUI_SOURCES ${IMGUI_ROOT_SOURCES_GLOB} ${IMGUI_IMPLEMENTATIONS_GLOB} PARENT_SCOPE)

function(include_imgui TARGET)
    target_sources(${TARGET} PRIVATE ${IMGUI_SOURCES})
    target_include_directories(${TARGET} PRIVATE ${IMGUI_INCLUDES})
endfunction()

set(GLM_ENABLE_CXX_20 ON CACHE BOOL "" FORCE)
add_subdirectory(glm-1.0.3)