#include "TestingMinimal.hpp"

int one() { return 1; }

TEST_CASE("doctest test")
{
    CHECK(one() == 1);
}