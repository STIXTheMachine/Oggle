#include "Application/Application.hpp"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

namespace Oggle {
Application::Application()
{
    glfwInit();

    MainWindow = std::make_unique<Window>();
    MainWindow->Create();
}

void Application::Run()
{
    while (!MainWindow->ShouldClose())
    {
        glfwPollEvents();
        MainWindow->Update();
    }
}

Application::~Application()
{
    MainWindow->Destroy();
    glfwTerminate();
}
} // Oggle