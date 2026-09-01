#include "TestingMinimal.hpp"
#include "Core/Vocabulary/Result.hpp"

using Oggle::Result;
using Oggle::EResultState;

#pragma region Helper Macros
#define CHECK_RESULT_IS_EMPTY(Res) do { \
        CHECK(Res.IsEmpty()); \
        CHECK_FALSE(Res.IsValid()); \
        CHECK_FALSE(Res.IsError()); \
        CHECK(Res.GetState() == EResultState::Empty); \
    } while(false)

#define CHECK_RESULT_IS_VALID(Res) do { \
        CHECK(Res.IsValid()); \
        CHECK_FALSE(Res.IsEmpty()); \
        CHECK_FALSE(Res.IsError()); \
        CHECK(Res.GetState() == EResultState::Valid); \
    } while(false)

#define CHECK_RESULT_IS_ERROR(Res) do { \
        CHECK(Res.IsError()); \
        CHECK_FALSE(Res.IsEmpty()); \
        CHECK_FALSE(Res.IsValid()); \
        CHECK(Res.GetState() == EResultState::Error); \
    } while(false)

#define REQUIRE_RESULT_IS_EMPTY(Res) do { \
        REQUIRE(Res.IsEmpty()); \
        REQUIRE_FALSE(Res.IsValid()); \
        REQUIRE_FALSE(Res.IsError()); \
        REQUIRE(Res.GetState() == EResultState::Empty); \
    } while(false)

#define REQUIRE_RESULT_IS_VALID(Res) do { \
        REQUIRE(Res.IsValid()); \
        REQUIRE_FALSE(Res.IsEmpty()); \
        REQUIRE_FALSE(Res.IsError()); \
        REQUIRE(Res.GetState() == EResultState::Valid); \
    } while(false)

#define REQUIRE_RESULT_IS_ERROR(Res) do { \
        REQUIRE(Res.IsError()); \
        REQUIRE_FALSE(Res.IsEmpty()); \
        REQUIRE_FALSE(Res.IsValid()); \
        REQUIRE(Res.GetState() == EResultState::Error); \
    } while(false)
#pragma endregion

TEST_CASE("Initialization")
{
    SUBCASE("Via constructor")
    {
        SUBCASE("Default constructed result is empty")
        {
            Result<int> Res;
            CHECK_RESULT_IS_EMPTY(Res);
        }

        SUBCASE("Result constructed from value type is valid.")
        {
            SUBCASE("Via constructor")
            {
                Result<int> Res { 42 };
                CHECK_RESULT_IS_VALID(Res);
                CHECK(Res.GetValue() == 42);
            }
            SUBCASE("Via FromValue()")
            {
                Result<int> Res = Result<int>::FromValue(42);
                CHECK_RESULT_IS_VALID(Res);
                CHECK(Res.GetValue() == 42);
            }
        }

        SUBCASE("Result constructed from error type is error")
        {
            SUBCASE("Via constructor")
            {
                Result<int> Res { "Have you ever heard the tale of Darth Plagueis the wise?" };
                CHECK_RESULT_IS_ERROR(Res);
            }
            SUBCASE("Via FromError()")
            {
                Result<int> Res = Result<int>::FromError("According to all known laws of aviation, there is no way a bee should be able to fly. Its wings are too small to get its fat little body off the ground.");
                CHECK_RESULT_IS_ERROR(Res);
            }
        }
    }

    SUBCASE("Copying")
    {
        SUBCASE("Copying empty result gives empty result")
        {
            Result<int> Res1;
            REQUIRE_RESULT_IS_EMPTY(Res1);

            Result<int> Res2 = Res1;
            CHECK_RESULT_IS_EMPTY(Res2);
        }

        SUBCASE("Copying valid result gives valid result with same value")
        {
            Result<int> Res1 { 42 };
            REQUIRE_RESULT_IS_VALID(Res1);

            Result<int> Res2 = Res1;
            CHECK_RESULT_IS_VALID(Res2);

            CHECK(Res1.GetValue() == Res2.GetValue());
        }

        SUBCASE("Copying errorful result gives errorful result with same error")
        {
            Result<int> Res1 { "Somebody once told me the world is gonna roll me" };
            REQUIRE_RESULT_IS_ERROR(Res1);

            Result<int> Res2 = Res1;
            CHECK_RESULT_IS_ERROR(Res2);

            CHECK(Res1.GetError() == Res2.GetError());
        }
    }

    SUBCASE("Moving")
    {
        SUBCASE("Moving empty result gives empty result, leaves source empty")
        {
            Result<int> Res1;
            REQUIRE_RESULT_IS_EMPTY(Res1);

            Result<int> Res2 = std::move(Res1);
            CHECK_RESULT_IS_EMPTY(Res1);
            CHECK_RESULT_IS_EMPTY(Res2);
        }

        SUBCASE("Moving valid result gives valid result with same value, leaves source empty")
        {
            Result<int> Res1 { 42 };
            REQUIRE_RESULT_IS_VALID(Res1);

            Result<int> Res2 = std::move(Res1);
            CHECK_RESULT_IS_EMPTY(Res1);
            CHECK_RESULT_IS_VALID(Res2);
            CHECK(Res2.GetValue() == 42);
        }

        SUBCASE("Moving errorful result gives errorful result with same error")
        {
            Result<int> Res1 { "You having a giggle mate?" };
            REQUIRE_RESULT_IS_ERROR(Res1);

            Result<int> Res2 = std::move(Res1);
            CHECK_RESULT_IS_EMPTY(Res1);
            CHECK_RESULT_IS_ERROR(Res2);
            CHECK(Res2.GetError() == "You having a giggle mate?");
        }
    }
}

// SUBCASE("Moving value from valid Result leaves Result empty")
// {
//     Result<int> Res { 42 };
//     REQUIRE_RESULT_IS_VALID(Res);
//
//     auto Val = Res.MoveValue();
//     CHECK_RESULT_IS_EMPTY(Res);
//     CHECK(Val == 42);
// }
//
// SUBCASE("Moving error from errorful Result leaves Result empty")
// {
//     Result<int> Res { "To be fair, you have to have a very high IQ to understand Rick and Morty." };
//     REQUIRE_RESULT_IS_ERROR(Res);
//
//     auto Err = Res.MoveError();
//     CHECK_RESULT_IS_EMPTY(Res);
//     CHECK(Err == "To be fair, you have to have a very high IQ to understand Rick and Morty.");
// }
