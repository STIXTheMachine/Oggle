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

#define SAME_ADDRESS(A, B) static_cast<const void*>(A) == static_cast<const void*>(B)

}
