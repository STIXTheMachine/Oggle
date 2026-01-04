#include "Application/Application.hpp"
#include "Windowing/SpawnTestWindow.hpp"

namespace Oggle {
Application::Application()
{
    SpawnGlfwWindow();
}
} // Oggle