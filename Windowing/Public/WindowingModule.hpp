#pragma once
#include "Core/ModuleInterface.hpp"

namespace Oggle
{
// TODO: Make an actual module system and make this participate in it
class WindowingModule //: ModuleInterface
{
    friend struct Application;
    static void Init();
    static void Shutdown();
};
}
