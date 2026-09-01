#pragma once
#include <Core/Logging/Logging.hpp>
DECLARE_LOG_CATEGORY(Assert, Fatal, Default);

namespace Oggle::Private
{
    inline void AssertImpl(bool Condition, std::string_view Message = "Assert failed!")
    {
        if (!Condition)
        {
            LOG(Assert, Message);
        }
    };
}

#define OGGLE_ASSERT(Condition) { Oggle::Private::AssertImpl(!!Condition); }
#define OGGLE_ASSERT_MSG(Condition, Message) { Oggle::Private::AssertImpl(!!Condition, Message); }