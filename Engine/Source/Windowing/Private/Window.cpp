#include "CoreMinimal.hpp"
#include "Windowing/Window.hpp"
#include "LogWindowing.hpp"
#include "GLFW/glfw3.h"

namespace Oggle {
WindowCreateResult Window::Create(const WindowCreateInfo& CreateInfo)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, CreateInfo.OpenGLVersionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, CreateInfo.OpenGLVersionMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, CreateInfo.bEnableDebug);

    if (NativeHandle NativeWindow = glfwCreateWindow(CreateInfo.Width, CreateInfo.Height, CreateInfo.Name.c_str(), nullptr, nullptr))
    {
        return WindowHandle { new Window { NativeWindow } };
    }

    const char* ErrDescription {};
    const auto ErrCode = glfwGetError(&ErrDescription);
    std::string ErrCodeReadable;

    switch (ErrCode)
    {
        case GLFW_NOT_INITIALIZED:
            ErrCodeReadable = "GLFW_NOT_INITIALIZED";
        case GLFW_NO_CURRENT_CONTEXT:
            ErrCodeReadable = "GLFW_NO_CURRENT_CONTEXT";
        case GLFW_INVALID_ENUM:
            ErrCodeReadable = "GLFW_INVALID_ENUM";
        case GLFW_INVALID_VALUE:
            ErrCodeReadable = "GLFW_INVALID_VALUE";
        case GLFW_OUT_OF_MEMORY:
            ErrCodeReadable = "GLFW_OUT_OF_MEMORY";
        case GLFW_API_UNAVAILABLE:
            ErrCodeReadable = "GLFW_API_UNAVAILABLE";
        case GLFW_VERSION_UNAVAILABLE:
            ErrCodeReadable = "GLFW_VERSION_UNAVAILABLE";
        case GLFW_PLATFORM_ERROR:
            ErrCodeReadable = "GLFW_PLATFORM_ERROR";
        case GLFW_FORMAT_UNAVAILABLE:
            ErrCodeReadable = "GLFW_FORMAT_UNAVAILABLE";
        case GLFW_NO_WINDOW_CONTEXT:
            ErrCodeReadable = "GLFW_NO_WINDOW_CONTEXT";
        default:
            ErrDescription = "Unknown Error";
            ErrCodeReadable = "Unknown Error";
    }

    return WindowCreateError
    {
        .ErrorCode = ErrCode,
        .ErrorCodeReadable = ErrCodeReadable,
        .ErrorDescription = ErrDescription
    };
}

Window::~Window()
{
    if (Handle)
    {
        glfwDestroyWindow(Handle);
    }
    Handle = nullptr;
}

void Window::MakeContextCurrent() const
{
    if (Handle)
    {
        glfwMakeContextCurrent(Handle);
    }
}

void Window::SwapBuffers() const
{
    glfwSwapBuffers(Handle);
}

bool Window::ShouldClose() const
{
    return glfwWindowShouldClose(Handle);
}

Window& Window::operator=(Window&& Other) noexcept
{
    this->Handle = Other.Handle;
    Other.Handle = nullptr;
    return *this;
}

Window::Window(Window&& Other) noexcept
{
    this->Handle = Other.Handle;
    Other.Handle = nullptr;
}

Window::Window(NativeHandle InHandle) : Handle(InHandle) {};
} // Oggle