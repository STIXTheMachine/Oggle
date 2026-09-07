#pragma once
#include "CoreMinimal.hpp"
#include <format>

namespace Oggle
{
    struct StringView final
    {
        using Char = char;
        StringView() = default;
        StringView(const Char* Str);

        [[nodiscard]] const Char* Data() const;
        [[nodiscard]] size_t Size() const;

    private:
         const Char* Buffer;
        size_t Length;
    };

    std::ostream& operator<<(std::ostream& Stream, StringView View);

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

        void DoubleCapacity();

        struct HeapString
        {
            Char* HeapBuffer  {};
            size_t Length {};
        };

        static constexpr size_t SmallStringCapacity = (sizeof(HeapString) / sizeof(Char)) - 1;

        struct StackString
        {
            Char StackBuffer[SmallStringCapacity] {};
            const Char Hardstop = '\0';
        };

        union Memory
        {
            StackString Stack {};
            HeapString Heap;
        } Rep;

        size_t m_Capacity;
    };

    std::ostream& operator<<(std::ostream& Stream, const String& String);
}

template<>
struct std::formatter<Oggle::String> : std::formatter<const char*> {
    auto format(Oggle::String& Str, auto& Ctx) const
    {
        return std::formatter<const char*>::format(Str.CStr(), Ctx);
    }
};

template<>
struct std::formatter<Oggle::StringView> : std::formatter<const char*> {
    auto format(Oggle::StringView& Str, auto& Ctx) const
    {
        return std::formatter<const char*>::format(Str.Data(), Ctx);
    }
};
