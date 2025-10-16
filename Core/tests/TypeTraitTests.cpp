#include <Core/TypeTraits.hpp>

static_assert(TypesAreUnique_v<int>);                       // single type → true
static_assert(TypesAreUnique_v<int, float>);                // distinct types → true
static_assert(TypesAreUnique_v<int, float, double>);        // multiple distinct → true
static_assert(TypesAreUnique_v<char, signed char, unsigned char>); // distinct despite similar names

// Fail cases: duplicates
static_assert(!TypesAreUnique_v<int, int>);                 // identical
static_assert(!TypesAreUnique_v<int, float, int>);          // duplicate later in pack
static_assert(!TypesAreUnique_v<float, double, float, double>); // multiple duplicates

// cv-qualifiers and references
static_assert(!TypesAreUnique_v<int, const int>);           // const doesn’t make it unique
static_assert(!TypesAreUnique_v<int, int&>);                // reference still same underlying type
static_assert(!TypesAreUnique_v<int, int&&>);               // same for rvalue ref
static_assert(!TypesAreUnique_v<int&, int&&>);              // both refer to int
static_assert(TypesAreUnique_v<int, const float&>);         // different base types

// Type aliases
using i32 = int;
static_assert(!TypesAreUnique_v<int, i32>);                 // aliases collapse to same type

// Fundamental edge cases
static_assert(TypesAreUnique_v<void>);                      // single void → true
static_assert(TypesAreUnique_v<void, std::nullptr_t>);      // distinct types → true
static_assert(!TypesAreUnique_v<void, void>);               // identical void → false

// Qualifier-mixed combos
static_assert(!TypesAreUnique_v<const int, volatile int>);  // cv-qualifiers ignored by is_same_v
static_assert(TypesAreUnique_v<const int, volatile float>); // different underlying types → true