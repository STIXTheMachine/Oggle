#pragma once
#include <Core/Logging/Logging.hpp>
#include <source_location>
#include <stacktrace>
DECLARE_LOG_CATEGORY(Assert, Fatal, Warning, Default);
DECLARE_LOG_CATEGORY(Ensure, Warning, Warning, Default);

#if defined(OGGLE_ENABLE_ASSERTS)
namespace Oggle::Private::Assert
{
    struct AssertInfo
    {
        std::string          ErrorMessage;
        std::source_location Location;
        std::stacktrace      Stacktrace;
    };

    [[noreturn]] void AssertImpl(AssertInfo& Info);

    void EnsureImpl(AssertInfo& Info);
}


#define OGGLE_ASSERT_2_ARGS(Condition, Message) \
{ \
    if (!(Condition)) \
    { \
        Oggle::Private::Assert::AssertInfo Info { .ErrorMessage = std::string { Message }, .Location = std::source_location::current(), .Stacktrace = std::stacktrace::current() }; \
        Oggle::Private::Assert::AssertImpl(Info); \
    } \
}

#define OGGLE_ASSERT_1_ARG(Condition) OGGLE_ASSERT_2_ARGS(Condition, "")

#define OGGLE_DISPATCH_ASSERT(_1, _2, NUM_ARGS, ...) NUM_ARGS

#define OGGLE_ASSERT(...) OGGLE_DISPATCH_ASSERT(__VA_ARGS__, OGGLE_ASSERT_2_ARGS, OGGLE_ASSERT_1_ARG)(__VA_ARGS__)



#define OGGLE_ENSURE_2_ARGS(Condition, Message) \
{ \
    if (!(Condition)) \
    { \
        Oggle::Private::Assert::AssertInfo Info { .ErrorMessage = std::string { Message }, .Location = std::source_location::current(), .Stacktrace = std::stacktrace::current() }; \
        Oggle::Private::Assert::EnsureImpl(Info); \
    } \
}

#define OGGLE_ENSURE_1_ARG(Condition) OGGLE_ENSURE_2_ARGS(Condition, "")

#define OGGLE_DISPATCH_ENSURE(_1, _2, NUM_ARGS, ...) NUM_ARGS

#define OGGLE_ENSURE(...) OGGLE_DISPATCH_ENSURE(__VA_ARGS__, OGGLE_ENSURE_2_ARGS, OGGLE_ENSURE_1_ARG)(__VA_ARGS__)


#else
#define OGGLE_ASSERT(Condition)
#define OGGLE_ASSERT_MSG(Condition, Message)
#define OGGLE_ENSURE(Condition, InMessage)
#define OGGLE_ENSURE_MSG(Condition, InMessage)
#endif