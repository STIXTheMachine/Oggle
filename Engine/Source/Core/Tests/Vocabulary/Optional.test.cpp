#include "TestingMinimal.hpp"
#include "Core/Vocabulary/Optional.hpp"
#include "Testing/Utilities/Canaries.hpp"

using namespace Oggle;

#define REQUIRE_OPTIONAL_IS_EMPTY(Optional)\
    REQUIRE_EQ(Optional.GetState(), EOptionalState::Empty);\
    REQUIRE(Optional.IsEmpty()); \
    REQUIRE_FALSE(Optional.IsValid());

#define REQUIRE_OPTIONAL_IS_VALID(Optional)\
    REQUIRE_EQ(Optional.GetState(), EOptionalState::Valid);\
    REQUIRE(Optional.IsValid());\
    REQUIRE_FALSE(Optional.IsEmpty());

#define CHECK_OPTIONAL_IS_EMPTY(Optional)\
    CHECK_EQ(Optional.GetState(), EOptionalState::Empty);\
    CHECK(Optional.IsEmpty());\
    CHECK_FALSE(Optional.IsValid());

#define CHECK_OPTIONAL_IS_VALID(Optional)\
    CHECK_EQ(Optional.GetState(), EOptionalState::Valid);\
    CHECK(Optional.IsValid());\
    CHECK_FALSE(Optional.IsEmpty());

TEST_SUITE_BEGIN("Optional");
TEST_CASE("Construction")
{
    SUBCASE("Default")
    {
        CHECK_OPTIONAL_IS_EMPTY(Optional<int> {});
    }
    SUBCASE("From ValueType")
    {
        SUBCASE("lvalue")
        {
            int Foo = 3;
            CHECK_OPTIONAL_IS_VALID(Optional<int> { Foo });
        }
        SUBCASE("rvalue")
        {
            REQUIRE_OPTIONAL_IS_VALID(Optional<int> { 2 });
        }
    }
    SUBCASE("Copy Constructor")
    {
        using ValueType = CopyMoveCanary<int>;

        Optional<ValueType> DefaultOptional { };
        REQUIRE_OPTIONAL_IS_EMPTY(DefaultOptional);

        Optional DefaultOptionalCopy { DefaultOptional };
        CHECK_OPTIONAL_IS_EMPTY(DefaultOptionalCopy);
        CHECK_OPTIONAL_IS_EMPTY(DefaultOptional);

        Optional<ValueType> ValidOptional { 3 };
        REQUIRE_OPTIONAL_IS_VALID(ValidOptional);

        Optional ValidOptionalCopy { ValidOptional };
        CHECK_OPTIONAL_IS_VALID(ValidOptionalCopy);
        CHECK_OPTIONAL_IS_VALID(ValidOptional);
        CHECK_EQ(ValidOptionalCopy.GetValue(), 3);
        CHECK_EQ(ValidOptional.GetValue(), 3);
    }
    SUBCASE("Move Constructor")
    {
        using ValueType = CopyMoveCanary<int>;

        Optional<ValueType> DefaultOptional { };
        REQUIRE_OPTIONAL_IS_EMPTY(DefaultOptional);

        Optional DefaultOptionalCopy { std::move(DefaultOptional) };
        CHECK_OPTIONAL_IS_EMPTY(DefaultOptionalCopy);
        CHECK_OPTIONAL_IS_EMPTY(DefaultOptional);

        Optional<ValueType> ValidOptional { 3 };
        REQUIRE_OPTIONAL_IS_VALID(ValidOptional);

        Optional ValidOptionalCopy { std::move(ValidOptional) };
        CHECK_OPTIONAL_IS_VALID(ValidOptionalCopy);
        CHECK_OPTIONAL_IS_VALID(ValidOptional);
        CHECK_EQ(ValidOptionalCopy.GetValue(), 3);
        CHECK_EQ(ValidOptional.GetValue().HasBeenMoved(), true);

    }
}

TEST_CASE("Assignment")
{
    SUBCASE("Copy Assignment")
    {
        SUBCASE("Self Assignment")
        {
            
        }
    }

    SUBCASE("Move Assignment") {}
}

TEST_CASE("State Query") {}

TEST_CASE("Access") {}

TEST_CASE("Mutation") {}

TEST_CASE("Extraction") {}

TEST_SUITE_END();
