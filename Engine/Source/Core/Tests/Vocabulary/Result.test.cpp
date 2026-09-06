#include "Core/Vocabulary/Result.hpp"
#include <memory>
#include "Testing/Utilities/Canaries.hpp"
#include "TestingMinimal.hpp"

using namespace Oggle;

#pragma region Helper Macros
// REQUIRE macros, used to enforce test preconditions
#define REQUIRE_RESULT_IS_EMPTY(Res)                     \
    do                                                   \
    {                                                    \
        REQUIRE(Res.IsEmpty());                          \
        REQUIRE_FALSE(Res.IsValid());                    \
        REQUIRE_FALSE(Res.IsError());                    \
        REQUIRE_EQ(Res.GetState(), EResultState::Empty); \
    }                                                    \
    while (false)

#define REQUIRE_RESULT_IS_VALID(Res)                     \
    do                                                   \
    {                                                    \
        REQUIRE(Res.IsValid());                          \
        REQUIRE_FALSE(Res.IsEmpty());                    \
        REQUIRE_FALSE(Res.IsError());                    \
        REQUIRE_EQ(Res.GetState(), EResultState::Valid); \
    }                                                    \
    while (false)

#define REQUIRE_RESULT_IS_ERROR(Res)                     \
    do                                                   \
    {                                                    \
        REQUIRE(Res.IsError());                          \
        REQUIRE_FALSE(Res.IsEmpty());                    \
        REQUIRE_FALSE(Res.IsValid());                    \
        REQUIRE_EQ(Res.GetState(), EResultState::Error); \
    }                                                    \
    while (false)

// CHECK macros, used to verify postconditions
#define CHECK_RESULT_IS_EMPTY(Res)                     \
    do                                                 \
    {                                                  \
        CHECK(Res.IsEmpty());                          \
        CHECK_FALSE(Res.IsValid());                    \
        CHECK_FALSE(Res.IsError());                    \
        CHECK_EQ(Res.GetState(), EResultState::Empty); \
    }                                                  \
    while (false)

#define CHECK_RESULT_IS_VALID(Res)                     \
    do                                                 \
    {                                                  \
        CHECK(Res.IsValid());                          \
        CHECK_FALSE(Res.IsEmpty());                    \
        CHECK_FALSE(Res.IsError());                    \
        CHECK_EQ(Res.GetState(), EResultState::Valid); \
    }                                                  \
    while (false)

#define CHECK_RESULT_IS_ERROR(Res)                     \
    do                                                 \
    {                                                  \
        CHECK(Res.IsError());                          \
        CHECK_FALSE(Res.IsEmpty());                    \
        CHECK_FALSE(Res.IsValid());                    \
        CHECK_EQ(Res.GetState(), EResultState::Error); \
    }                                                  \
    while (false)
#pragma endregion

TEST_SUITE_BEGIN("Result");
TEST_CASE("Construction")
{
    SUBCASE("Default")
    {
        Result<int> Empty;
        CHECK_RESULT_IS_EMPTY(Empty); // Default constructed Result is empty
    }

    SUBCASE("From Value")
    {
        SUBCASE("lvalue")
        {
            int         Val { 42 };
            Result<int> LValueConstructed { Val };
            CHECK_RESULT_IS_VALID(LValueConstructed);    // Result constructed from Value is Valid
            CHECK_EQ(LValueConstructed.GetValue(), Val); // Value equals value from which the Result was constructed
        }

        SUBCASE("rvalue")
        {
            Result<int> RValueConstructed { 42 };
            CHECK_RESULT_IS_VALID(RValueConstructed);   // Result constructed from Value is Valid
            CHECK_EQ(RValueConstructed.GetValue(), 42); // Value equals value from which the Result was constructed
        }
    }

    SUBCASE("From Error")
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

    SUBCASE("Copy Construction")
    {
        using CopyTestResult = Result<CopyMoveCanary<int>, CopyMoveCanary<float>>;

        CopyTestResult InitialEmpty;
        REQUIRE_RESULT_IS_EMPTY(InitialEmpty);

        CopyTestResult InitialValid { CopyMoveCanary<int> { 42 } };
        REQUIRE_RESULT_IS_VALID(InitialValid);

        CopyTestResult InitialError { CopyMoveCanary<float> { 69.f } };
        REQUIRE_RESULT_IS_ERROR(InitialError);

        CopyTestResult CopyConstructEmpty { InitialEmpty };
        CHECK_RESULT_IS_EMPTY(CopyConstructEmpty); // Copying empty Result gives empty result
        CHECK_RESULT_IS_EMPTY(InitialEmpty);       // Empty state of copied-from Result is preserved

        CopyTestResult CopyConstructValid { InitialValid };
        CHECK_RESULT_IS_VALID(CopyConstructValid); // Copying valid result gives valid result
        CHECK_RESULT_IS_VALID(InitialValid);       // Valid state of copied-from Result is preserved
        CHECK_EQ(CopyConstructValid.GetValue().WrappedValue, InitialValid.GetValue().WrappedValue);
        // Value of new copy equals Value of old copy
        CHECK_EQ(InitialValid.GetValue().WrappedValue, 42); // Value of old copy is unchanged
        CHECK(InitialValid.GetValue().HasBeenCopied());     // Operation induced a copy of Value

        CopyTestResult CopyConstructError { InitialError };
        CHECK_RESULT_IS_ERROR(CopyConstructError); // Copying errorful Result gives errorful Result
        CHECK_RESULT_IS_ERROR(InitialError);       // Errorful state of copied-from Result is preserved
        CHECK_EQ(CopyConstructError.GetError().WrappedValue, InitialError.GetError().WrappedValue);
        // Error of new copy equals Error of old copy
        CHECK_EQ(InitialError.GetError().WrappedValue, 69.f); // Error of old copy is unchanged
        CHECK(InitialError.GetError().HasBeenCopied());       // Operation induced a copy of Error
    }

    SUBCASE("Move Constructor")
    {
        using MoveTestResult = Result<CopyMoveCanary<int*>, CopyMoveCanary<float*>>;

        int  ValueResource    = 42;
        int* ValueResourcePtr = &ValueResource;

        float  ErrorResource    = 69.0f;
        float* ErrorResourcePtr = &ErrorResource;

        MoveTestResult InitialEmpty;
        REQUIRE_RESULT_IS_EMPTY(InitialEmpty);

        MoveTestResult InitialValid { CopyMoveCanary<int*> { ValueResourcePtr } };
        REQUIRE_RESULT_IS_VALID(InitialValid);

        MoveTestResult InitialError { CopyMoveCanary<float*> { ErrorResourcePtr } };
        REQUIRE_RESULT_IS_ERROR(InitialError);

        MoveTestResult MoveConstructEmpty { std::move(InitialEmpty) };
        CHECK_RESULT_IS_EMPTY(MoveConstructEmpty); // Moving empty Result gives empty result
        CHECK_RESULT_IS_EMPTY(InitialEmpty);       // Empty state of moved-from Result is preserved

        MoveTestResult MoveConstructValid { std::move(InitialValid) };
        CHECK_RESULT_IS_VALID(MoveConstructValid); // Moving valid result gives valid result
        CHECK_RESULT_IS_VALID(InitialValid);       // Valid state of moved-from Result is preserved
        CHECK_EQ(MoveConstructValid.GetValue().WrappedValue, ValueResourcePtr);
        // Moved-to Value should hold same resource as moved-from Value did
        CHECK(InitialValid.GetValue().HasBeenMoved()); // Operation induced a move of Value

        MoveTestResult MoveConstructError { std::move(InitialError) };
        CHECK_RESULT_IS_ERROR(MoveConstructError); // Moving errorful Result gives errorful Result
        CHECK_RESULT_IS_ERROR(InitialError);       // Errorful state of moved-from Result is preserved
        CHECK_EQ(MoveConstructError.GetError().WrappedValue, ErrorResourcePtr);
        // Moved-to Error should hold the same resource as the moved-from Error did
        CHECK(InitialError.GetError().HasBeenMoved()); // Operation induced a move of Error
    }
} // Construction

TEST_CASE("Assignment")
{
    SUBCASE("Copy Assignment")
    {
        using CopyTestResult = Result<CopyMoveCanary<int>, CopyMoveCanary<float>>;

        CopyTestResult InitialEmpty;
        REQUIRE_RESULT_IS_EMPTY(InitialEmpty);

        CopyTestResult InitialValid { CopyMoveCanary<int> { 42 } };
        REQUIRE_RESULT_IS_VALID(InitialValid);

        CopyTestResult InitialError { CopyMoveCanary<float> { 69.f } };
        REQUIRE_RESULT_IS_ERROR(InitialError);

        CopyTestResult CopyAssignEmpty;
        CopyAssignEmpty = InitialEmpty;
        CHECK_RESULT_IS_EMPTY(CopyAssignEmpty); // Copying empty Result gives empty result
        CHECK_RESULT_IS_EMPTY(InitialEmpty);    // Empty state of copied-from Result is preserved

        CopyTestResult CopyAssignValid;
        CopyAssignValid = InitialValid;
        CHECK_RESULT_IS_VALID(CopyAssignValid); // Copying valid result gives valid result
        CHECK_RESULT_IS_VALID(InitialValid);    // Valid state of copied-from Result is preserved
        CHECK_EQ(CopyAssignValid.GetValue().WrappedValue, InitialValid.GetValue().WrappedValue);
        // Value of new copy equals Value of old copy
        CHECK_EQ(InitialValid.GetValue().WrappedValue, 42); // Value of old copy is unchanged
        CHECK(InitialValid.GetValue().HasBeenCopied());     // Operation induced a copy of Value

        CopyTestResult CopyAssignError;
        CopyAssignError = InitialError;
        CHECK_RESULT_IS_ERROR(CopyAssignError); // Copying errorful Result gives errorful Result
        CHECK_RESULT_IS_ERROR(InitialError);    // Errorful state of copied-from Result is preserved
        CHECK_EQ(CopyAssignError.GetError().WrappedValue, InitialError.GetError().WrappedValue);
        // Error of new copy equals Error of old copy
        CHECK_EQ(InitialError.GetError().WrappedValue, 69.f); // Error of old copy is unchanged
        CHECK(InitialError.GetError().HasBeenCopied());       // Operation induced a copy of Error
    }

    SUBCASE("Move Assignment")
    {
        using MoveTestResult = Result<CopyMoveCanary<int*>, CopyMoveCanary<float*>>;

        int  ValueResource    = 42;
        int* ValueResourcePtr = &ValueResource;

        float  ErrorResource    = 69.0f;
        float* ErrorResourcePtr = &ErrorResource;

        MoveTestResult InitialEmpty;
        REQUIRE_RESULT_IS_EMPTY(InitialEmpty);

        MoveTestResult InitialValid { CopyMoveCanary<int*> { ValueResourcePtr } };
        REQUIRE_RESULT_IS_VALID(InitialValid);

        MoveTestResult InitialError { CopyMoveCanary<float*> { ErrorResourcePtr } };
        REQUIRE_RESULT_IS_ERROR(InitialError);

        MoveTestResult MoveAssignEmpty;
        MoveAssignEmpty = std::move(InitialEmpty);
        CHECK_RESULT_IS_EMPTY(MoveAssignEmpty); // Moving empty Result gives empty result
        CHECK_RESULT_IS_EMPTY(InitialEmpty);    // Empty state of moved-from Result is preserved

        MoveTestResult MoveAssignValid;
        MoveAssignValid = std::move(InitialValid);
        CHECK_RESULT_IS_VALID(MoveAssignValid); // Moving valid result gives valid result
        CHECK_RESULT_IS_VALID(InitialValid);    // Valid state of moved-from Result is preserved
        CHECK_EQ(MoveAssignValid.GetValue().WrappedValue, ValueResourcePtr);
        // Moved-to Value should hold same resource as moved-from Value did
        CHECK(InitialValid.GetValue().HasBeenMoved()); // Operation induced a move of Value

        MoveTestResult MoveAssignError;
        MoveAssignError = std::move(InitialError);
        CHECK_RESULT_IS_ERROR(MoveAssignError); // Moving errorful Result gives errorful Result
        CHECK_RESULT_IS_ERROR(InitialError);    // Errorful state of moved-from Result is preserved
        CHECK_EQ(MoveAssignError.GetError().WrappedValue, ErrorResourcePtr);
        // Moved-to Error should hold the same resource as the moved-from Error did
        CHECK(InitialError.GetError().HasBeenMoved()); // Operation induced a move of Error
    }
} // Assignment

TEST_CASE("State queries")
{
    Result<int> EmptyResult;
    REQUIRE_RESULT_IS_EMPTY(EmptyResult);

    Result<int> ValidResult { 42 };
    REQUIRE_RESULT_IS_VALID(ValidResult);

    Result<int> ErrorResult { "Hello" };
    REQUIRE_RESULT_IS_ERROR(ErrorResult);

    SUBCASE("IsEmpty()")
    {
        CHECK_RESULT_IS_EMPTY(EmptyResult);
    }
    SUBCASE("IsValid()")
    {
        CHECK_RESULT_IS_VALID(ValidResult);
    }
    SUBCASE("IsError()")
    {
        CHECK_RESULT_IS_ERROR(ErrorResult);
    }

    SUBCASE("operator bool()")
    {
        CHECK_FALSE(EmptyResult);
        CHECK(ValidResult);
        CHECK_FALSE(ErrorResult);
    }
} // State Queries

TEST_CASE("Access")
{
    SUBCASE("GetValue()")
    {
        Result<int> ValidResult { 42 };
        REQUIRE_RESULT_IS_VALID(ValidResult);

        const Result<int> ConstValidResult { 42 };
        REQUIRE_RESULT_IS_VALID(ConstValidResult);

        CHECK_EQ(ValidResult.GetValue(), 42);
        CHECK_EQ(ConstValidResult.GetValue(), 42);
    }

    SUBCASE("GetError()")
    {
        Result<int> ErrorResult { "Hello" };
        REQUIRE_RESULT_IS_ERROR(ErrorResult);

        const Result<int> ConstErrorResult { "Hello" };
        REQUIRE_RESULT_IS_ERROR(ConstErrorResult);

        CHECK_EQ(ErrorResult.GetError(), "Hello");
        CHECK_EQ(ConstErrorResult.GetError(), "Hello");
    }

    SUBCASE("GetValueOr())")
    {
        Result<int> EmptyResult { };
        REQUIRE_RESULT_IS_EMPTY(EmptyResult);

        Result<int> ValidResult { 42 };
        REQUIRE_RESULT_IS_VALID(ValidResult);

        Result<int> ErrorResult { "Hello" };
        REQUIRE_RESULT_IS_ERROR(ErrorResult);

        CHECK_EQ(EmptyResult.GetValueOr(69), 69);
        CHECK_EQ(ValidResult.GetValueOr(69), 42);
        CHECK_EQ(ErrorResult.GetValueOr(69), 69);
    }

    SUBCASE("GetErrorOr()")
    {
        Result<int> EmptyResult { };
        REQUIRE_RESULT_IS_EMPTY(EmptyResult);

        Result<int> ValidResult { 42 };
        REQUIRE_RESULT_IS_VALID(ValidResult);

        Result<int> ErrorResult { "Hello" };
        REQUIRE_RESULT_IS_ERROR(ErrorResult);

        CHECK_EQ(EmptyResult.GetErrorOr("Goodbye"), "Goodbye");
        CHECK_EQ(ValidResult.GetErrorOr("Goodbye"), "Goodbye");
        CHECK_EQ(ErrorResult.GetErrorOr("Goodbye"), "Hello");
    }
} // Access

TEST_CASE("Mutation")
{
    SUBCASE("SetValue()")
    {
        Result<int> InitialEmpty { };
        REQUIRE_RESULT_IS_EMPTY(InitialEmpty);

        Result<int> InitialValid { 42 };
        REQUIRE_RESULT_IS_VALID(InitialValid);

        Result<int> InitialError { "Hello" };
        REQUIRE_RESULT_IS_ERROR(InitialError);

        InitialEmpty.SetValue(69);
        CHECK_RESULT_IS_VALID(InitialEmpty);
        CHECK_EQ(InitialEmpty.GetValue(), 69);

        InitialValid.SetValue(69);
        CHECK_RESULT_IS_VALID(InitialValid);
        CHECK_EQ(InitialValid.GetValue(), 69);

        InitialError.SetValue(69);
        CHECK_RESULT_IS_VALID(InitialError);
        CHECK_EQ(InitialError.GetValue(), 69);
    }

    SUBCASE("SetError()")
    {
        Result<int> InitialEmpty { };
        REQUIRE_RESULT_IS_EMPTY(InitialEmpty);

        Result<int> InitialValid { 42 };
        REQUIRE_RESULT_IS_VALID(InitialValid);

        Result<int> InitialError { "Hello" };
        REQUIRE_RESULT_IS_ERROR(InitialError);

        InitialEmpty.SetError("Goodbye");
        CHECK_RESULT_IS_ERROR(InitialEmpty);
        CHECK_EQ(InitialEmpty.GetError(), "Goodbye");

        InitialValid.SetError("Goodbye");
        CHECK_RESULT_IS_ERROR(InitialValid);
        CHECK_EQ(InitialValid.GetError(), "Goodbye");

        InitialError.SetError("Goodbye");
        CHECK_RESULT_IS_ERROR(InitialError);
        CHECK_EQ(InitialError.GetError(), "Goodbye");
    }

    SUBCASE("Clear()")
    {
        Result<int> InitialEmpty { };
        REQUIRE_RESULT_IS_EMPTY(InitialEmpty);

        Result<int> InitialValid { 42 };
        REQUIRE_RESULT_IS_VALID(InitialValid);

        Result<int> InitialError { "Hello" };
        REQUIRE_RESULT_IS_ERROR(InitialError);

        InitialEmpty.Clear();
        CHECK_RESULT_IS_EMPTY(InitialEmpty);

        InitialValid.Clear();
        CHECK_RESULT_IS_EMPTY(InitialValid);

        InitialError.Clear();
        CHECK_RESULT_IS_EMPTY(InitialError);
    }
} // Mutation

TEST_CASE("Extraction")
{
    using TakeTestResult = Result<CopyMoveCanary<int>, CopyMoveCanary<const char*>>;

    SUBCASE("TakeValue()")
    {
        TakeTestResult ValidResult { CopyMoveCanary<int> { 42 } };
        REQUIRE_RESULT_IS_VALID(ValidResult);

        CopyMoveCanary<int> MovedValue = ValidResult.TakeValue();
        CHECK_RESULT_IS_VALID(ValidResult);
        CHECK_EQ(MovedValue.WrappedValue, 42);
        CHECK(ValidResult.GetValue().HasBeenMoved());
    }

    SUBCASE("TakeError()")
    {
        TakeTestResult ErrorResult { CopyMoveCanary<const char*> { "Hello" } };
        REQUIRE_RESULT_IS_ERROR(ErrorResult);

        CopyMoveCanary<const char*> MovedError = ErrorResult.TakeError();
        CHECK_RESULT_IS_ERROR(ErrorResult);
        CHECK_EQ(MovedError.WrappedValue, "Hello");
        CHECK(ErrorResult.GetError().HasBeenMoved());
    }
} // Extraction
TEST_SUITE_END();
