#include "CoreMinimal.hpp"

struct Foo
{
    //Foo() { Val = 1; };
    Foo() = delete;

    int Val;
};

static_assert(std::is_default_constructible_v<Foo>);

using namespace Oggle;

int main()
{
    Result<Foo, char> MyResult { EResultForceInit::DoNotForce } ;

    if (MyResult)
    {
        std::cout << MyResult.GetValue().Val;
    }
}