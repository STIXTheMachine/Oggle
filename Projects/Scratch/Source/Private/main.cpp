#include "CoreMinimal.hpp"
#include <string>
#include <print>

using namespace Oggle;

void stop() {};

int main()
{
    String MyString { "WOOOOOOO YEAH THIS IS MY OWN STRING CLASS." };

    stop();

    std::cout << MyString << std::endl;
}