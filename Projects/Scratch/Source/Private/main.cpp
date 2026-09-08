#include "CoreMinimal.hpp"
using namespace Oggle;

const char* Data[] = { "Foo", "Bar", "Baz", "Qux" };

Span<const char*> MyView { Data };

int main()
{
    std::cout << sizeof(Optional<char>);
}