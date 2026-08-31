#pragma once
#include "Core/Logging/Logging.hpp"

namespace Oggle::Private
{
    inline void AssertImpl(bool Condition, std::string_view Message = "Assert failed!")
    {
        if (!Condition)
        {
            //LOG(Assert, Message);
            std::abort();
        }
    };
}

#define OGGLE_ASSERT(Condition) { Oggle::Private::AssertImpl(!!Condition); }
//#define OGGLE_ASSERTF(Condition, Message) { Oggle::Private::AssertImpl(!!Condition, Message); }