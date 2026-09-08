#include "CoreMinimal.hpp"
using namespace Oggle;

const char* Data[] = { "Foo", "Bar", "Baz", "Qux" };

Span<const char*> MyView { Data };

int main()
{
    for (const auto Elem : MyView)
    {
        LOG(Elem);
    }
}