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


#define OGGLE_ASSERT_MSG(Condition, Message) \
{ \
    if (!(Condition)) \
    { \
        Oggle::Private::Assert::AssertInfo Info { .ErrorMessage = std::string { Message }, .Location = std::source_location::current(), .Stacktrace = std::stacktrace::current() }; \
        Oggle::Private::Assert::AssertImpl(Info); \
    } \
}
#define OGGLE_ASSERT(Condition) OGGLE_ASSERT_MSG(Condition, "")


#define OGGLE_ENSURE_MSG(Condition, Message) \
{ \
    if (!(Condition)) \
    { \
        Oggle::Private::Assert::AssertInfo Info { .ErrorMessage = std::string { Message }, .Location = std::source_location::current(), .Stacktrace = std::stacktrace::current() }; \
        Oggle::Private::Assert::EnsureImpl(Info); \
    } \
}
#define OGGLE_ENSURE(Condition) OGGLE_ENSURE_MSG(Condition, "")


#else
#define OGGLE_ASSERT(Condition)
#define OGGLE_ASSERT_MSG(Condition, Message)
#define OGGLE_ENSURE(Condition, InMessage)
#define OGGLE_ENSURE_MSG(Condition, InMessage)
#endif