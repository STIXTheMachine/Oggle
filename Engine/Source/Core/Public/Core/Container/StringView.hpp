#pragma once
#include "Core/Vocabulary/PrimitiveTypes.hpp"
#include "Core/Container/Span.hpp"

namespace Oggle
{
    struct StringView
    {
        using Char = char;

        /// @brief
        //StringView(const String&);

    private:
        Span<const Char> m_Data;
    };
}