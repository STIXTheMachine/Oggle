#include "CoreMinimal.hpp"
#include "../../../Application/Public/Application/Application.hpp"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glbinding/gl/gl.h"
#include "glbinding/glbinding.h"

int main()
{
    using namespace gl;

    Oggle::Application App;
    App.Run();

    // glfwInit();
    //
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //
    // #ifdef DEBUG_OPENGL
    // glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE)
    // #endif
    //
    // auto Window = glfwCreateWindow(1600, 900, "Window", nullptr, nullptr);
    //
    // if (Window == nullptr)
    // {
    //     LOG("Failed to create GLFW window!")
    //     std::abort(); // TODO: Make assert library and replace
    // }
    //
    // glfwMakeContextCurrent(Window);
    // glbinding::initialize(glfwGetProcAddress);
    //
    // glfwShowWindow(Window);
    //
    // auto Background = Oggle::FloatColor::Blue;
    // glClearColor(Background.R, Background.G, Background.B, Background.A);
    //
    // while (!glfwWindowShouldClose(Window))
    // {
    //     glfwPollEvents();
    //
    //     glClear(GL_COLOR_BUFFER_BIT);
    //
    //     glfwSwapBuffers(Window);
    // }
    //
    //
    // glfwTerminate();
}