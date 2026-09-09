#pragma once
#include "StringView.hpp"
#include <format>

namespace Oggle
{
    namespace Detail
    {
        struct StackString;
        struct HeapString
        {
            HeapString() = default;
            HeapString(size_t Capacity);
            HeapString(const Char* String);
            HeapString(const StackString& Other);

            HeapString(const HeapString& Other);
            HeapString(HeapString&& Other);
            HeapString& operator=(const HeapString& Other);
            HeapString& operator=(HeapString&& Other);

            ~HeapString();

            static Char* Allocate(size_t Capacity);
            static void Deallocate(Char* Data);
            Char* Reallocate(size_t NewCapacity);

            Char* Buffer {};
            size_t Length {};
        };

        static constexpr size_t SmallStringBufferSize = sizeof(HeapString) / sizeof(Char); // Size of largest string that can fit in SmallStringBufSize
        static constexpr size_t SmallStringCapacity   = SmallStringBufferSize - 1;

        struct StackString
        {
            StackString() = default;
            StackString(const Char* String);
            size_t Length() const;

            Char Buffer[SmallStringBufferSize] {};
        };
    }

    struct String final
    {
        using Char = char;

        /// @brief Default constructor
        String();

        /// @brief Create a string with space for NumChars characters. (Does not initialize underlying memory)
        /// @param Capacity Number of characters
        String(size_t Capacity);

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

        /// @brief Construct a String from a StringView
        /// @param View The View to copy for this string
        String(StringView View);

        /// @brief Copy assignment operator
        /// @param Other String to copy
        /// @return reference to this
        String& operator=(const String& Other);

        /// @brief Move assignment operator. Returns Other to a default-constructed state.
        /// @param Other String to move from
        /// @return reference to this
        String& operator=(String&& Other);

        /// @brief Concatenation assignment operator
        /// @return String containing the concatenation of the two parameters
        String& operator+=(const Char* Rhs);
        String& operator+=(const String& Rhs);
        String& operator+=(StringView Rhs);

        /// @brief Concatenation operator
        /// @return String containing the concatenation of the two parameters
        friend String operator+(String Lhs, const String& Rhs);
        friend String operator+(String Lhs, const Char* Rhs);
        friend String operator+(String Lhs, StringView Rhs);

        String& operator=(StringView View);

        /// @brief Ensure that the String has room for at least NumChars characters, reallocating if necessary.
        /// @param NumChars Desired new capacity
        void Reserve(size_t NumChars);

        /// Get string contents as a regular C string
        [[nodiscard]] const Char* CStr() const;

        bool operator==(const String& Other) const;
        bool operator==(const Char* Other) const;
        bool operator==(StringView Other) const;

        Char* Data();

        [[nodiscard]] size_t Length() const;
        [[nodiscard]] size_t Capacity() const;
        [[nodiscard]] bool IsEmpty() const;

        friend std::ostream& operator<<(std::ostream& Stream, const Oggle::String& String);

    private:
        [[nodiscard]] bool IsSmall() const;

        /// @brief Grow the string
        void IncreaseCapacity();

        union Memory
        {
            Detail::StackString Stack {};
            Detail::HeapString Heap;
            ~Memory() {};
        } Rep;

        size_t m_Capacity { Detail::SmallStringCapacity };
    };

}


template<>
struct std::formatter<Oggle::String> : std::formatter<const char*> {
    auto format(Oggle::String& Str, auto& Ctx) const
    {
        return std::formatter<const char*>::format(Str.CStr(), Ctx);
    }
};
