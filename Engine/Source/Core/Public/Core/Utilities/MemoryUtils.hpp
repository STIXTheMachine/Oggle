#pragma once
#include "Core/Vocabulary/OggleType.hpp"

namespace Oggle
{
    inline void* ZeroMemory(void* Location, size_t NumBytes)
    {
        auto Byte = static_cast<char*>(Location);
        while (NumBytes--) *Byte++ = 0;
        return Location;
    };
}
