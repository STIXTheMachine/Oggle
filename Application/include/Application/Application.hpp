#pragma once
#include <memory>
#include "Windowing/Window.hpp"

namespace Oggle
{
struct Application
{
    Application();

    void Run();

    ~Application();
private:
    std::unique_ptr<Window> MainWindow;
};
} // Oggle