#pragma once
#include <cstdlib>

namespace Oggle::Private
{
    inline void AssertImpl(bool Condition)
    {
        if (!Condition)
        {
            std::abort();
        }
    };
}

#define OGGLE_ASSERT(Condition) { Oggle::Private::AssertImpl(!!Condition); }