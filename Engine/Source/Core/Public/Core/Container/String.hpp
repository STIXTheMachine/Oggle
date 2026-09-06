#pragma once
#include "CoreMinimal.hpp"

namespace Oggle
{
    struct String final
    {
        using CharT = char;
        String() = default;

    private:
        struct HeapString
        {
            CharT* Buffer {};
            size_t Length {};
        };

        static constexpr size_t SmallStringThreshold = (sizeof(HeapString) / sizeof(CharT)) - 1;

        struct StackString
        {
            CharT Buffer[SmallStringThreshold] {};
        };

        union StringMemory
        {
            StackString Stack;
            HeapString Heap;
            StringMemory() : Stack() {};
            ~StringMemory() = default;
        } InternalValue;

        size_t Capacity {};

        [[nodiscard]] bool IsSmallString() const;
    };
}