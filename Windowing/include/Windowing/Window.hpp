#pragma once

struct GLFWwindow;

namespace Oggle
{
struct Window
{
    Window() = default;
    ~Window();

    void Create();
    void Update();
    void Destroy();

    bool ShouldClose() const;

    GLFWwindow* GetNativeHandle() const { return NativeHandle; }
private:
    GLFWwindow* NativeHandle;
};
} // Oggle