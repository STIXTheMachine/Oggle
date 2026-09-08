#pragma once
#include "Core/Vocabulary/PrimitiveTypes.hpp"
#include "Core/Container/Span.hpp"

namespace Oggle
{
    struct StringView : public Span<char>
    {
        using Char = char;


    private:
        Span<Char> View;
    };
}