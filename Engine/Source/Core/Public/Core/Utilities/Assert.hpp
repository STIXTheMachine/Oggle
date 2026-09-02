#pragma once
#include <Core/Logging/Logging.hpp>
#include <source_location>
#include <stacktrace>
DECLARE_LOG_CATEGORY(Assert, Fatal, Default);

#if defined(OGGLE_ENABLE_ASSERTS)
namespace Oggle::Assert::Private
{
    struct AssertInfo
    {
        std::string          Message;
        std::source_location Location   = std::source_location::current();
        std::stacktrace      Stacktrace = std::stacktrace::current();
    };

    [[noreturn]] void AssertImpl(bool Condition, AssertInfo& Info);

    void EnsureImpl(bool Condition, AssertInfo& Info);
}

#define OGGLE_ASSERT(Condition) { Oggle::Assert::Private::AssertInfo Info; Oggle::Assert::Private::AssertImpl(!!Condition, Info); }
#define OGGLE_ASSERT_MSG(Condition, InMessage) { Oggle::Assert::Private::AssertInfo Info { .Message = std::string { InMessage } }; Oggle::Assert::Private::AssertImpl(!!Condition, Info); }
#define OGGLE_ENSURE(Condition) { Oggle::Assert::Private::AssertInfo Info; Oggle::Assert::Private::EnsureImpl(!!Condition, Info); }
#define OGGLE_ENSURE_MSG(Condition, InMessage) { Oggle::Assert::Private::AssertInfo Info { .Message = std::string { InMessage } }; Oggle::Assert::Private::EnsureImpl(!!Condition, Info); }
#else
#define OGGLE_ASSERT(Condition)
#define OGGLE_ASSERT_MSG(Condition, Message)
#define OGGLE_ENSURE(Condition, InMessage)
#define OGGLE_ENSURE_MSG(Condition, InMessage)
#endif