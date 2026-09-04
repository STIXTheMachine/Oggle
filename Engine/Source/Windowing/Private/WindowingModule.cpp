#include "WindowingModule.hpp"
#include "CoreMinimal.hpp"
#include "GLFW/glfw3.h"
#include "LogWindowing.hpp"

void Oggle::WindowingModule::Init()
{
    using namespace Oggle;
    LOG(Windowing, "Initializing windowing library...");
    glfwInit();
}

void Oggle::WindowingModule::Shutdown()
{
    LOG(Windowing, "Shutting down windowing library...");
    glfwTerminate();
}
