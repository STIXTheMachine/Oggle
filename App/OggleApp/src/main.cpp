#include "Application/Application.hpp"

int main()
{
    Oggle::Application App;
    App.Init();
    App.Run();
    App.Shutdown();
}
