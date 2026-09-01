#pragma once
#include <CoreMinimal.hpp>

struct GLFWwindow;


namespace Oggle
{

struct Window;

struct WindowCreateInfo
{
    uint32_t    Width              = 1920;
    uint32_t    Height             = 1080;
    std::string Name               = "Window";
    uint8_t     OpenGLVersionMajor = 4;
    uint8_t     OpenGLVersionMinor = 6;
    bool        bEnableDebug       = OGGLE_DEBUG_ENABLED;
};

struct WindowCreateError
{
    int ErrorCode;
    std::string ErrorCodeReadable;
    std::string ErrorDescription;
};

using WindowHandle = Unique<Window>;
using WindowCreateResult = Result<WindowHandle, WindowCreateError>;

struct Window
{
    using NativeHandle = GLFWwindow*;

    static WindowCreateResult Create(const WindowCreateInfo& CreateInfo);
    ~Window();

    void MakeContextCurrent() const;
    void SwapBuffers() const;
    bool ShouldClose() const;

    Window& operator=(const Window&) = delete;
    Window(const Window&)            = delete;

    Window& operator=(Window&&) noexcept;
    Window(Window&&)            noexcept;

private:
    Window(NativeHandle);
    NativeHandle Handle {};
};

} // Oggle