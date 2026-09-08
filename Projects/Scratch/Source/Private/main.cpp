#include "CoreMinimal.hpp"
#include <debugging>

using namespace Oggle;


static void Foo()
{
    const char* Foo[5] = { "Foo", "Bar", "Baz", "Qux", "Glim" };
    View MyView { Foo, std::size(Foo) };

    for (const auto Element : MyView)
    {
        LOG(Element);
    }
}

int main()
{
    Foo();
}