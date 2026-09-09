#include "Container/String.hpp"
#include <cstring>

namespace Oggle
{
String::String()
{
    InitSmallString();
}

String::String(size_t NumChars)
{
    if (NumChars <= SmallStringCapacity)
    {
        InitSmallString();
    }
    else
    {
        InitBigString(NumChars);
    }
}

String::String(size_t NumChars, Char Fill)
{
    if (IsSmall())
    {
        InitSmallString();
        memset(Rep.Stack, Fill, SmallStringCapacity);
        Rep.Stack[NumChars] = '\0';
    }
    else
    {
        InitBigString(NumChars);
        memset(Rep.Heap.Data, Fill, NumChars);
        Rep.Heap.Data[NumChars] = '\0';
    }
}

String::String(const char* String)
{
    const size_t Length = strlen(String);

    if (Length <= SmallStringCapacity)
    {
        InitSmallString();
        strncpy(Rep.Stack, String, SmallStringCapacity);
        Rep.Stack[Length] = '\0';
    }
    else
    {
        InitBigString(Length);
        strncpy(Rep.Heap.Data, String, Length);
        Rep.Heap.Data[Length] = '\0';
        Rep.Heap.Length = Length;
    }
}

String::String(const String& Other)
{
    if (Other.IsSmall())
    {
        InitSmallString();
        strncpy(Rep.Stack, Other.Rep.Stack, SmallStringCapacity);
        Rep.Stack[Other.Length()] = '\0';
    }
    else
    {
        InitBigString(Other.Capacity());
    }
}

String::String(String&& Other)
{
    if (Other.IsSmall())
    {
        InitSmallString();
        strncpy(Rep.Stack, Other.Rep.Stack, SmallStringCapacity);
    }
    else
    {
        m_Capacity     = Other.Capacity();
        Rep.Heap       = Other.Rep.Heap;
        Other.Rep.Heap = {};
    }
}

String::String(StringView View)
{
    OGGLE_UNIMPLEMENTED();
}

String& String::operator=(const String& Other)
{
    if (this == &Other) return *this;

    if (IsSmall())
    {
        DestroySmallString();
    }
    else {
        DestroyBigString();
    }

    if (Other.IsSmall())
    {
        InitSmallString();
        strncpy(Rep.Stack, Other.Rep.Stack, SmallStringCapacity);
    }
    else
    {
        InitBigString(Other.Capacity());
        strncpy(Rep.Heap.Data, Other.Rep.Heap.Data, Other.Capacity());
    }

    return *this;
}


String& String::operator=(String&& Other)
{
    if (this == &Other) return *this;

    if (IsSmall())
    {
        DestroySmallString();
    }
    else {
        DestroyBigString();
    }

    if (Other.IsSmall())
    {
        InitSmallString();
        strncpy(Rep.Stack, Other.Rep.Stack, SmallStringCapacity);
    }
    else
    {
        m_Capacity     = Other.Capacity();
        Rep.Heap       = Other.Rep.Heap;
        Other.Rep.Heap = {};
    }

    return *this;
}

String& String::operator+=(const Char* Rhs)
{
    const size_t InitialLength    = Length();
    const size_t ExtraLength      = strlen(Rhs);
    const size_t RequiredCapacity = InitialLength + ExtraLength;

    Reserve(RequiredCapacity);

    if (IsSmall())
    {
        Char*        CurrentStringEnd = Rep.Stack + Length(); // Points to null terminator of string in Rep.Stack
        const size_t FreeBufferLength = SmallStringCapacity - InitialLength;
        strncpy(CurrentStringEnd, Rhs, FreeBufferLength);
        Rep.Stack[RequiredCapacity] = '\0';
    }
    else
    {
        Char*        CurrentStringEnd = Rep.Heap.Data + Length(); // Points to null terminator
        const size_t FreeBufferLength = Capacity() - InitialLength;
        strncpy(CurrentStringEnd, Rhs, FreeBufferLength);
        Rep.Heap.Data[RequiredCapacity] = '\0'; // Until Optional<T&> is supported
        //Rep.Heap[RequiredCapacity].GetValue() = '\0';
    }

    return *this;
}

String& String::operator+=(const String& Rhs)
{
    return *this += Rhs.CStr();
}

String& String::operator+=(StringView Rhs)
{
    // TODO: Implement once StringView is a thing again
    OGGLE_UNIMPLEMENTED();
    return *this;
}

String& String::operator=(StringView View)
{
    OGGLE_UNIMPLEMENTED();
    return *this;
}

void String::Reserve(size_t NumChars)
{
    if (NumChars <= Capacity())
    {
        return;
    }

    // Capacity() should never return anything less than SmallStringCapacity, so if we get here we for sure need to allocate
    Char* const  NewBuffer    = Allocate(NumChars);
    const size_t CachedLength = Length();

    if (IsSmall())
    {
        memcpy(NewBuffer, Rep.Stack, SmallStringCapacity);
        DestroySmallString();
        Rep.Heap = { .Data = NewBuffer, .Length = CachedLength };
    }
    else
    {
        const Char*  OldBuffer    = Rep.Heap.Data;
        memcpy(NewBuffer, Rep.Heap.Data, CachedLength);
        delete[] OldBuffer;
        Rep.Heap = { .Data = NewBuffer, .Length = CachedLength };
    }

    m_Capacity = NumChars;
}

const String::Char* String::CStr() const
{
    if (IsSmall()) return Rep.Stack;

    return Rep.Heap.Data;
}
bool String::operator==(const String& Other) const
{
    const size_t MinLength = Length() < Other.Length() ? Length() : Other.Length();
    return strncmp(CStr(), Other.CStr(), MinLength) == 0;
}
bool String::operator==(const Char* Other) const
{
    const size_t MinLength = Length() < strlen(Other) ? Length() : strlen(Other);
    return strncmp(CStr(), Other, MinLength) == 0;
}
bool String::operator==(StringView Other) const
{
    OGGLE_UNIMPLEMENTED()
    return false;
}

String::Char* String::Data()
{
    if (IsSmall()) return Rep.Stack;

    return Rep.Heap.Data;
}

size_t String::Length() const
{
    if (IsSmall()) return strlen(Rep.Stack);

    return Rep.Heap.Length;
}

size_t String::Capacity() const
{
    OGGLE_ENSURE(m_Capacity >= SmallStringCapacity, FMT("String::Capacity() reported a capacity of {}, which is smaller than the SSO capacity of {}. This doesn't inherently break anything but should be investigated.", m_Capacity, SmallStringCapacity));

    return m_Capacity;
}
bool String::IsEmpty() const
{
    return Length() == 0;
}

void String::InitSmallString() {
    std::construct_at(&Rep.Stack);
    Rep.Stack[SmallStringCapacity] = '\0';
    m_Capacity                     = SmallStringCapacity;
}

void String::DestroySmallString()
{
    std::destroy_at(&Rep.Stack);
}

void String::InitBigString(size_t Capacity)
{
    auto Buffer      = Allocate(Capacity);
    Buffer[Capacity] = '\0';
    std::construct_at(&Rep.Heap, Buffer, 0);
    m_Capacity = Capacity;
}

void String::DestroyBigString()
{
    delete[] Rep.Heap.Data;
    std::destroy_at(&Rep.Heap);
}

String::Char* String::Allocate(size_t NumChars)
{
    const auto Buffer = new Char[NumChars + 1]; // Include space for a null terminator
    OGGLE_ASSERT(Buffer != nullptr);
    return Buffer;
}

void String::Deallocate(Char* Buffer)
{
    delete[] Buffer;
}

bool String::IsSmall() const
{
    return Capacity() <= SmallStringCapacity;
}

void String::IncreaseCapacity()
{
    Reserve(2 * Capacity());
}

String operator+(String Lhs, const String& Rhs)
{
    Lhs += Rhs;
    return Lhs;
}

String operator+(String Lhs, const Char* Rhs)
{
    Lhs += Rhs;
    return Lhs;
}

String operator+(String Lhs, StringView Rhs)
{
    Lhs += Rhs;
    return Lhs;
}


std::ostream& operator<<(std::ostream& Stream, const Oggle::String& String)
{
    return Stream << String.CStr();
}
} // namespace Oggle
