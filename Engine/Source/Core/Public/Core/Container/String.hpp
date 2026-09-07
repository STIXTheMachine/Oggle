#pragma once
#include "StringView.hpp"
#include <format>

namespace Oggle
{
    struct String final
    {
        using Char = char;

        String();

        /// Create a string with space for NumChars characters. (Underlying memory is uninitialized.)
        String(size_t NumChars);

        /// Create a string composed ot NumChars Fill characters
        String(size_t NumChars, Char Fill);

        /// Create a String out of a C string
        String(const char* String);

        String(const String& Other);
        String(String&& Other);

        String& operator=(const String& Other);
        String& operator=(String&& Other);

        String& operator+=(const Char* String);
        String& operator+=(const String& Other);
        String& operator+=(StringView Other);

        String(StringView View);
        String& operator=(StringView View);

        /// Sets Capacity to be at least NumChars characters, reallocating if necessary.
        void Reserve(size_t NumChars);

        StringView View();

        /// Get string contents as a regular C string
        [[nodiscard]] const Char* CStr() const;

        Char* Data();

        [[nodiscard]] size_t Size() const;
        [[nodiscard]] size_t Capacity() const;

    private:
        // Allocate space for NumChars characters plus a null terminator
        static Char* Allocate(size_t NumChars);
        static void Deallocate(Char*);

        [[nodiscard]] bool IsSmallString() const;

        // Allocates a new heap buffer of size 2 * m_Capacity and then copies the existing buffer into it
        void DoubleCapacity();

        struct HeapBuffer
        {
            Char* Buf  {};
            size_t Length {}; // Includes null byte
        };

        // Includes space for the null terminator
        static constexpr size_t SmallStringBufSize = (sizeof(HeapBuffer) / sizeof(Char));

        struct StackBuffer
        {
            Char Buf[SmallStringBufSize] {}; // Includes null byte
        };

        union Memory
        {
            StackBuffer Stack {};
            HeapBuffer Heap;
        } Rep;

        size_t m_Capacity;
    };

}

std::ostream& operator<<(std::ostream& Stream, const Oggle::String& String);

template<>
struct std::formatter<Oggle::String> : std::formatter<const char*> {
    auto format(Oggle::String& Str, auto& Ctx) const
    {
        return std::formatter<const char*>::format(Str.CStr(), Ctx);
    }
};
