#pragma once
#include "Core/Vocabulary/PrimitiveTypes.hpp"
#include "Core/Container/View.hpp"

namespace Oggle
{
    struct StringView : public View<char>
    {
        using Char = char;


    private:
        View<Char> View;
    };
}