#include "CoreMinimal.hpp"
#include "Windowing/Window.hpp"
#include "Windowing/LogWindowing.hpp"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glbinding/gl/gl.h"
#include "glbinding/glbinding.h"

using namespace gl;

namespace Oggle {
Window::~Window()
{
    Destroy();
}

void Window::Create()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef DEBUG_OPENGL
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE)
    #endif

    NativeHandle = glfwCreateWindow(1600, 900, "Window", nullptr, nullptr);

    if (NativeHandle == nullptr)
    {
        LOG(Windowing, Error, "Failed to create GLFW window!")
        std::abort(); // TODO: Make assert library and replace
    }

    glfwMakeContextCurrent(NativeHandle);
    glbinding::initialize(glfwGetProcAddress);
}

void Window::Update()
{
    auto Background = Oggle::FloatColor::Emerald;
    glClearColor(Background.R, Background.G, Background.B, Background.A);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(NativeHandle);
}

void Window::Destroy()
{
    if (NativeHandle)
    {
        glfwDestroyWindow(NativeHandle);
        NativeHandle = nullptr;
    }
}

bool Window::ShouldClose() const
{
    return glfwWindowShouldClose(NativeHandle);
}
} // Oggle