#pragma once
#include "Windowing/Window.hpp"
#include "Renderer/Renderer.hpp"

namespace Oggle
{

struct ApplicationInitInfo
{
    std::string Name;
};

struct Application
{
    void Init(const ApplicationInitInfo& = ApplicationInitInfo {});
    void Run();
    void Shutdown();

private:
    void CreateMainWindow(WindowCreateInfo& WindowInfo);
    void CreateRenderer(RendererCreateInfo& RendererInfo);

    WindowHandle MainWindow;
    Unique<Renderer> Renderer;
};
} // Oggle