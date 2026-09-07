#pragma once
#include "CoreMinimal.hpp"

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

        /// Sets Capacity to be at least NumChars characters, reallocating if necessary.
        void Reserve(size_t NumChars);

        /// Get string contents as a regular C string
        const char* CStr() const;

        size_t GetLength() const;
        size_t GetCapacity() const;

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

        size_t Capacity;
    };

    std::ostream& operator<<(std::ostream& Stream, const String& String);
}