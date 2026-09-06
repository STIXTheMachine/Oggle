#include "Core/Container/String.hpp"

namespace Oggle
{
bool String::IsSmallString() const
{
    return Capacity <= SmallStringThreshold;
}
}
