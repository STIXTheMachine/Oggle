#pragma once
#include "StringView.hpp"
#include <format>

namespace Oggle
{
    struct String final
    {
        using Char = char;

        /// @brief Default constructor
        String();

        /// @brief Create a string with space for NumChars characters. (Does not initialize underlying memory)
        /// @param NumChars Number of characters
        String(size_t NumChars);

        /// @brief Create a string with space for NumChars characters, filling the entire buffer with the Fill character
        /// @param NumChars Number of characters
        /// @param Fill Character to fill with
        String(size_t NumChars, Char Fill);

        /// @brief Create a String from a C string
        /// @param String C string to copy
        String(const char* String);

        /// @brief Copy constructor
        /// @param Other string to copy
        String(const String& Other);

        /// @brief Move constructor. Returns Other to a default-constructed state.
        /// @param Other String to move from
        String(String&& Other);

        /// @brief Copy assignment operator
        /// @param Other String to copy
        /// @return reference to this
        String& operator=(const String& Other);

        /// @brief Move assignment operator. Returns Other to a default-constructed state.
        /// @param Other String to move from
        /// @return reference to this
        String& operator=(String&& Other);

        /// @brief Concatenation operator.
        /// Usage: String A = "Hello, "; String B = "World!"; String C = A + B; // C == "Hello, World!"
        /// @return String
        String& operator+(const String& Rhs);

        String& operator+=(const Char* String);
        String& operator+=(const String& Other);
        String& operator+=(StringView Other);

        String(StringView View);
        String& operator=(StringView View);

        /// @brief Ensure that the String has room for at least NumChars characters, reallocating if necessary.
        /// @param NumChars Desired new capacity
        void Reserve(size_t NumChars);

        /// Get string contents as a regular C string
        [[nodiscard]] const Char* CStr() const;

        Char* Data();

        [[nodiscard]] size_t Size() const;
        [[nodiscard]] size_t Capacity() const;

    private:
        /// @brief Allocate a null-terminated buffer with space for a string of capacity NumChars
        /// @param NumChars number of characters
        /// @return Pointer to allocated buffer
        static Char* Allocate(size_t NumChars);

        /// @brief Deallocate buffer previously allocated with String::Allocate
        static void Deallocate(Char*);

        [[nodiscard]] bool IsSmall() const;

        /// @brief Grow the string
        void IncreaseCapacity();

        struct HeapBuffer
        {
            Char* Buf  {};
            size_t Length {}; // Includes null byte
        };

        static constexpr size_t SmallStringBufSize  = sizeof(HeapBuffer) / sizeof(Char); // Size of buffer, including null byte
        static constexpr size_t SmallStringCapacity = SmallStringBufSize - 1;            // Size of largest string that can fit in SmallStringBufSize

        union Memory
        {
            Char Stack[SmallStringBufSize] {};
            Span<Char> Heap;
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
