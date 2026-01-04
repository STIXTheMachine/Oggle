#include "Windowing/SpawnTestWindow.hpp"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glbinding/gl/gl.h"
#include "glbinding/glbinding.h"
#include "Windowing/LogWindowing.hpp"
// #include "imgui.h"
// #define IMGUI_IMPL_GLFW_DISABLE_X11
// #include "imgui_impl_glfw.h"
// #include "imgui_impl_opengl3.h"

using namespace gl;

void SpawnGlfwWindow()
{
    if (!glfwInit())
    {
        LOG(Windowing, Error, "Failed to initialize GLFW!")
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    auto Window = glfwCreateWindow(1600, 900, "Hello, World!", nullptr, nullptr);

    glfwMakeContextCurrent(Window);

    glbinding::initialize(glfwGetProcAddress);

    while (!glfwWindowShouldClose(Window))
    {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(Window);
    }

    glfwTerminate();
}


// void SpawnImguiDemoWindow()
// {
//     if (!glfwInit())
//     {
//         LOG(Windowing, Error, "Failed to initialize GLFW!")
//         return;
//     }
//
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
//
//     auto Window = glfwCreateWindow(1600, 900, "Hello, World!", nullptr, nullptr);
//
//     glfwMakeContextCurrent(Window);
//
//     glbinding::initialize(glfwGetProcAddress);
//
//     // // Setup Dear ImGui context
//     IMGUI_CHECKVERSION();
//     ImGui::CreateContext();
//     ImGuiIO& io = ImGui::GetIO();
//     io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
//     io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
//     io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
//
//     // Setup Platform/Renderer backends
//     ImGui_ImplGlfw_InitForOpenGL(Window, true);               // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
//     ImGui_ImplOpenGL3_Init();
//
//     while (!glfwWindowShouldClose(Window))
//     {
//         glfwPollEvents();
//
//         ImGui_ImplOpenGL3_NewFrame();
//         ImGui_ImplGlfw_NewFrame();
//         ImGui::NewFrame();
//         ImGui::ShowDemoWindow();
//
//
//         glClear(GL_COLOR_BUFFER_BIT);
//
//         ImGui::Render();
//         ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//
//         glfwSwapBuffers(Window);
//     }
//
//     ImGui_ImplOpenGL3_Shutdown();
//     ImGui_ImplGlfw_Shutdown();
//     ImGui::DestroyContext();
//
//     glfwTerminate();
// }
