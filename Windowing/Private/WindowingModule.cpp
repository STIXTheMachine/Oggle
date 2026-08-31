#include "WindowingModule.hpp"
#include "CoreMinimal.hpp"
#include "GLFW/glfw3.h"
#include "../Public/LogWindowing.hpp"

void Oggle::WindowingModule::Init()
{
    LOG(Windowing, "Initializing windowing library...");
    glfwInit();
}

void Oggle::WindowingModule::Shutdown()
{
    LOG(Windowing, "Shutting down windowing library...");
    glfwTerminate();
}
