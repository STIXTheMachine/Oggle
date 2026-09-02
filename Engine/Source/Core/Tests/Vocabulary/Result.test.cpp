#include "TestingMinimal.hpp"
#include "Core/Vocabulary/Result.hpp"
#include "Testing/Utilities/MovedFromCanary.hpp"
#include <memory>

using namespace Oggle;

#pragma region Helper Macros
// REQUIRE macros, used to enforce test preconditions
#define REQUIRE_RESULT_IS_EMPTY(Res) do { \
        REQUIRE(Res.IsEmpty()); \
        REQUIRE_FALSE(Res.IsValid()); \
        REQUIRE_FALSE(Res.IsError()); \
        REQUIRE_EQ(Res.GetState(), EResultState::Empty); \
    } while(false)

#define REQUIRE_RESULT_IS_VALID(Res) do { \
        REQUIRE(Res.IsValid()); \
        REQUIRE_FALSE(Res.IsEmpty()); \
        REQUIRE_FALSE(Res.IsError()); \
        REQUIRE_EQ(Res.GetState(), EResultState::Valid); \
    } while(false)

#define REQUIRE_RESULT_IS_ERROR(Res) do { \
        REQUIRE(Res.IsError()); \
        REQUIRE_FALSE(Res.IsEmpty()); \
        REQUIRE_FALSE(Res.IsValid()); \
        REQUIRE_EQ(Res.GetState(), EResultState::Error); \
    } while(false)

// CHECK macros, used to verify postconditions
#define CHECK_RESULT_IS_EMPTY(Res) do { \
        CHECK(Res.IsEmpty()); \
        CHECK_FALSE(Res.IsValid()); \
        CHECK_FALSE(Res.IsError()); \
        CHECK_EQ(Res.GetState(), EResultState::Empty); \
    } while(false)

#define CHECK_RESULT_IS_VALID(Res) do { \
        CHECK(Res.IsValid()); \
        CHECK_FALSE(Res.IsEmpty()); \
        CHECK_FALSE(Res.IsError()); \
        CHECK_EQ(Res.GetState(), EResultState::Valid); \
    } while(false)

#define CHECK_RESULT_IS_ERROR(Res) do { \
        CHECK(Res.IsError()); \
        CHECK_FALSE(Res.IsEmpty()); \
        CHECK_FALSE(Res.IsValid()); \
        CHECK_EQ(Res.GetState(), EResultState::Error); \
    } while(false)
#pragma endregion


TEST_CASE("Constructing")
{
    SUBCASE("Default Constructor")
    {
        Result<int> Empty;
        CHECK_RESULT_IS_EMPTY(Empty); // Default constructed Result is empty
    }

    SUBCASE("Constructing From Values")
    {
        SUBCASE("lvalue")
        {
            int Val { 42 };
            Result<int> LValueConstructed { Val };
            CHECK_RESULT_IS_VALID(LValueConstructed);    // Result constructed from Value is Valid
            CHECK_EQ(LValueConstructed.GetValue(), Val); // Value equals value from which the Result was constructed
        }

        SUBCASE("rvalue")
        {
            Result<int> RValueConstructed { 42 };
            CHECK_RESULT_IS_VALID(RValueConstructed);    // Result constructed from Value is Valid
            CHECK_EQ(RValueConstructed.GetValue(), 42);  // Value equals value from which the Result was constructed
        }
    }

    SUBCASE("Constructing From Errors")
    {
        SUBCASE("lvalue")
        {
            std::string Err { "Hello" };

            Result<int> LValueConstructed { Err };
            CHECK_RESULT_IS_ERROR(LValueConstructed);        // Result constructed from Error is errorful
            CHECK_EQ(LValueConstructed.GetError(), "Hello"); // Error equals value from which the Result was constructed
        }

        SUBCASE("rvalue")
        {
            Result<int> RValueConstructed { "Hello" };
            CHECK_RESULT_IS_ERROR(RValueConstructed);        // Result constructed from Error is errorful
            CHECK_EQ(RValueConstructed.GetError(), "Hello"); // Error equals value from which the Result was constructed
        }
    }

    SUBCASE("Copying")
    {
        Result<int> InitialEmpty;
        REQUIRE_RESULT_IS_EMPTY(InitialEmpty);

        Result<int> InitialValid { 42 };
        REQUIRE_RESULT_IS_VALID(InitialValid);

        Result<int> InitialError { "Hello" };
        REQUIRE_RESULT_IS_ERROR(InitialError);

        SUBCASE("Copy Construction")
        {
            Result<int> CopyConstructEmpty { InitialEmpty };
            CHECK_RESULT_IS_EMPTY(CopyConstructEmpty);                        // Copying empty Result gives empty result
            CHECK_RESULT_IS_EMPTY(InitialEmpty);                              // Empty state of copied-from Result is preserved

            Result<int> CopyConstructValid { InitialValid };
            CHECK_RESULT_IS_VALID(CopyConstructValid);                        // Copying valid result gives valid result
            CHECK_RESULT_IS_VALID(InitialValid);                              // Valid state of copied-from Result is preserved
            CHECK_EQ(CopyConstructValid.GetValue(), InitialValid.GetValue()); // Value of new copy equals Value of old copy
            CHECK_EQ(InitialValid.GetValue(), 42);                            // Value of old copy is unchanged

            Result<int> CopyConstructError { InitialError };
            CHECK_RESULT_IS_ERROR(CopyConstructError);                        // Copying errorful Result gives errorful Result
            CHECK_RESULT_IS_ERROR(InitialError);                              // Errorful state of copied-from Result is preserved
            CHECK_EQ(CopyConstructError.GetError(), InitialError.GetError()); // Error of new copy equals Error of old copy
            CHECK_EQ(InitialError.GetError(), "Hello");                       // Error of old copy is unchanged
        }

        SUBCASE("Copy Assignment")
        {
            Result<int> CopyAssignEmpty;
            CopyAssignEmpty = InitialEmpty;
            CHECK_RESULT_IS_EMPTY(CopyAssignEmpty);                           // Copying empty Result gives empty result
            CHECK_RESULT_IS_EMPTY(InitialEmpty);                              // Empty state of copied-from Result is preserved

            Result<int> CopyAssignValid;
            CopyAssignValid = InitialValid;
            CHECK_RESULT_IS_VALID(CopyAssignValid);                           // Copying valid result gives valid result
            CHECK_RESULT_IS_VALID(InitialValid);                              // Valid state of copied-from Result is preserved
            CHECK_EQ(CopyAssignValid.GetValue(), InitialValid.GetValue());    // Value of new copy equals Value of old copy
            CHECK_EQ(InitialValid.GetValue(), 42);                            // Value of old copy is unchanged

            Result<int> CopyAssignError;
            CopyAssignError = InitialError;
            CHECK_RESULT_IS_ERROR(CopyAssignError);                           // Copying errorful Result gives errorful Result
            CHECK_RESULT_IS_ERROR(InitialError);                              // Errorful state of copied-from Result is preserved
            CHECK_EQ(CopyAssignError.GetError(), InitialError.GetError());    // Error of new copy equals Error of old copy
            CHECK_EQ(InitialError.GetError(), "Hello");                       // Error of old copy is unchanged
        }
    }

    SUBCASE("Moving")
    {
        using MoveTestResult = Result<MovedFromCanary<int*>, MovedFromCanary<float*>>;

        int  ValueResource    = 42;
        int* ValueResourcePtr = &ValueResource;

        float  ErrorResource    = 69.0f;
        float* ErrorResourcePtr = &ErrorResource;

        SUBCASE("Move Construction")
        {
            MoveTestResult InitialEmpty;
            REQUIRE_RESULT_IS_EMPTY(InitialEmpty);

            MoveTestResult InitialValid { ValueResourcePtr };
            REQUIRE_RESULT_IS_VALID(InitialValid);

            MoveTestResult InitialError { ErrorResourcePtr };
            REQUIRE_RESULT_IS_ERROR(InitialError);

            MoveTestResult MoveConstructEmpty { std::move(InitialEmpty) };
            CHECK_RESULT_IS_EMPTY(MoveConstructEmpty);                                // Moving empty Result gives empty result
            CHECK_RESULT_IS_EMPTY(InitialEmpty);                                      // Empty state of moved-from Result is preserved

            MoveTestResult MoveConstructValid { std::move(InitialValid) };
            CHECK_RESULT_IS_VALID(MoveConstructValid);                                // Moving valid result gives valid result
            CHECK_RESULT_IS_VALID(InitialValid);                                      // Valid state of moved-from Result is preserved
            CHECK_EQ(MoveConstructValid.GetValue().WrappedValue, ValueResourcePtr);   // Moved-to Value should hold same resource as moved-from Value did
            CHECK_EQ(InitialValid.GetValue().bMovedFrom, true);                       // Moved-from Value should be in a moved-from state

            MoveTestResult MoveConstructError { std::move(InitialError) };
            CHECK_RESULT_IS_ERROR(MoveConstructError);                                // Moving errorful Result gives errorful Result
            CHECK_RESULT_IS_ERROR(InitialError);                                      // Errorful state of moved-from Result is preserved
            CHECK_EQ(MoveConstructError.GetError().WrappedValue, ErrorResourcePtr);   // Moved-to Error should hold the same resource as the moved-from Error did
            CHECK_EQ(InitialError.GetError().bMovedFrom, true);                       // Moved-from Error should be in a moved-from state
        }

        SUBCASE("Move Assignment")
        {
            MoveTestResult InitialEmpty;
            REQUIRE_RESULT_IS_EMPTY(InitialEmpty);

            MoveTestResult InitialValid { ValueResourcePtr };
            REQUIRE_RESULT_IS_VALID(InitialValid);

            MoveTestResult InitialError { ErrorResourcePtr };
            REQUIRE_RESULT_IS_ERROR(InitialError);

            MoveTestResult MoveAssignEmpty;
            MoveAssignEmpty = std::move(InitialEmpty);
            CHECK_RESULT_IS_EMPTY(MoveAssignEmpty);                                   // Moving empty Result gives empty result
            CHECK_RESULT_IS_EMPTY(InitialEmpty);                                      // Empty state of moved-from Result is preserved

            MoveTestResult MoveAssignValid;
            MoveAssignValid = std::move(InitialValid);
            CHECK_RESULT_IS_VALID(MoveAssignValid);                                   // Moving valid result gives valid result
            CHECK_RESULT_IS_VALID(InitialValid);                                      // Valid state of moved-from Result is preserved
            CHECK_EQ(MoveAssignValid.GetValue().WrappedValue, ValueResourcePtr);      // Moved-to Value should hold same resource as moved-from Value did
            CHECK_EQ(InitialValid.GetValue().bMovedFrom, true);                       // Moved-from Value should be in a moved-from state

            MoveTestResult MoveAssignError;
            MoveAssignError = std::move(InitialError);
            CHECK_RESULT_IS_ERROR(MoveAssignError);                                   // Moving errorful Result gives errorful Result
            CHECK_RESULT_IS_ERROR(InitialError);                                      // Errorful state of moved-from Result is preserved
            CHECK_EQ(MoveAssignError.GetError().WrappedValue, ErrorResourcePtr);      // Moved-to Error should hold the same resource as the moved-from Error did
            CHECK_EQ(InitialError.GetError().bMovedFrom, true);                       // Moved-from Error should be in a moved-from state
        }
    }
}