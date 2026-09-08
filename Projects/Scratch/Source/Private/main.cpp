#include "CoreMinimal.hpp"
using namespace Oggle;

const char* Data[] = { "Foo", "Bar", "Baz", "Qux" };

Span<const char*> MyView { Data };

int main()
{
    String Foo;
    for (const auto Elem : MyView)
    {
        LOG(Elem);
    }
}