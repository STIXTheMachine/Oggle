#pragma once
#include <Core/Logging/Logging.hpp>
#include <source_location>
DECLARE_LOG_CATEGORY(Assert, Fatal, Default);

namespace Oggle::Private
{
    inline void AssertMsgImpl(bool Condition, std::string_view Message, std::source_location Location)
    {
        if (!Condition)
        {
            std::string LocationString = std::format(
                "{}:{}:{} in function {}",
                Location.file_name(),
                Location.line(),
                Location.column(),
                Location.function_name()
            );
            LOG(Assert, FMT("Assertion failed: {}\nat {}", Message, LocationString));
        }
    };

    inline void AssertImpl(bool Condition, std::source_location Location)
    {
        if (!Condition)
        {
            std::string LocationString = std::format(
                "{}:{}:{} in function {}",
                Location.file_name(),
                Location.line(),
                Location.column(),
                Location.function_name()
            );
            LOG(Assert, FMT("Assertion failed at {}", LocationString));
        }
    }
}

#define OGGLE_ASSERT(Condition) { std::source_location Location = std::source_location::current(); Oggle::Private::AssertImpl(!!Condition, Location); }
#define OGGLE_ASSERT_MSG(Condition, Message) { std::source_location Location = std::source_location::current(); Oggle::Private::AssertMsgImpl(!!Condition, Message, Location); }