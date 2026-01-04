#include "CoreMinimal.hpp"
#include "Renderer/LogRenderer.hpp"
#include "Windowing/LogWindowing.hpp"

int main()
{
    LOG("Hello, World!");
    LOG(Renderer, "Hello, Renderer!")
    LOG(Windowing, "Hello, Windowing!")
}