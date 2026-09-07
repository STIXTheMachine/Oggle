#include "CoreMinimal.hpp"
#include <debugging>

using namespace Oggle;

int main()
{
    std::string Foo { "Error Message" };
    OGGLE_ENSURE(false && true);
    OGGLE_ENSURE(false && true, Foo);
}