#include "CoreMinimal.hpp"

using namespace Oggle;

void stop() {};

int main()
{
    String MyString { "WOOOOOOO YEAH THIS IS MY OWN STRING CLASS." };

    stop();

    std::cout << std::format("Hello, {}", MyString.View() ) << std::endl;
}