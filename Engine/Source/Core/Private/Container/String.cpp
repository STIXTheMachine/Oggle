#include "Container/String.hpp"
#include <cstring>

Oggle::String::String()
{
    InitSmallString();
}

Oggle::String::String(size_t NumChars)
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

Oggle::String::String(size_t NumChars, Char Fill)
{
    if (IsSmall())
    {
        InitSmallString();
        memset(Rep.Stack, Fill, SmallStringCapacity);
    }
    else
    {
        InitBigString(NumChars);
        memset(Rep.Heap.Data(), Fill, NumChars);
    }
}

Oggle::String::String(const char* String)
{
    const size_t Length = strlen(String);

    if (Length <= SmallStringCapacity)
    {
        InitSmallString();
        strncpy(Rep.Stack, String, SmallStringCapacity);
    }
    else
    {
        InitBigString(Length);
        strncpy(Rep.Heap.Data(), String, Length);
    }
}

Oggle::String::String(const String& Other)
{
    if (Other.IsSmall())
    {
        InitSmallString();
        m_Capacity = SmallStringCapacity;
    }
}

Oggle::String::String(String&& Other)
{
    if (Other.IsSmall())
    {
        InitSmallString();
        strncpy(Rep.Stack, Other.Rep.Stack, SmallStringCapacity);
    }
    else
    {
        m_Capacity = Other.Capacity();
        Rep.Heap = Other.Rep.Heap;
        Other.Rep.Heap = {};
    }
}

Oggle::String& Oggle::String::operator=(const String& Other)
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
        strncpy(Rep.Heap.Data(), Other.Rep.Heap.Data(), Other.Capacity());
    }

    return *this;
}

Oggle::String& Oggle::String::operator=(String&& Other)
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
        m_Capacity = Other.Capacity();
        Rep.Heap = Other.Rep.Heap;
        Other.Rep.Heap = {};
    }

    return *this;
}

Oggle::String& Oggle::String::operator+(const String& Rhs)
{
    OGGLE_UNIMPLEMENTED();
    return *this += Rhs;
}

Oggle::String& Oggle::String::operator+=(const Char* String)
{
    const size_t InitialLength = Length();
    const size_t ExtraLength = strlen(String);
    const size_t RequiredCapacity = InitialLength + ExtraLength;

    Reserve(RequiredCapacity);

    if (IsSmall())
    {
        Char* CurrentStringEnd = Rep.Stack + Length(); // Points to null terminator of string in Rep.Stack
        const size_t FreeBufferLength = SmallStringCapacity - InitialLength;
        strncpy(CurrentStringEnd, String, FreeBufferLength);
        Rep.Stack[RequiredCapacity] = '\0';
    }
    else
    {
        Char* CurrentStringEnd = Rep.Heap.Data() + Length(); // Points to null terminator
        const size_t FreeBufferLength = Capacity() - InitialLength;
        strncpy(CurrentStringEnd, String, FreeBufferLength);
        //Rep.Heap[RequiredCapacity].GetValue() = '\0';
    }

    return *this;
}

Oggle::String& Oggle::String::operator+=(const String& Other)
{
    OGGLE_UNIMPLEMENTED();
    return *this;
}

Oggle::String& Oggle::String::operator+=(StringView Other)
{
    OGGLE_UNIMPLEMENTED();
    return *this;
}

Oggle::String::String(StringView View)
{
    OGGLE_UNIMPLEMENTED();
}

Oggle::String& Oggle::String::operator=(StringView View)
{
    OGGLE_UNIMPLEMENTED();
    return *this;
}

const Oggle::String::Char* Oggle::String::CStr() const
{
    if (IsSmall()) return Rep.Stack;

    return Rep.Heap.Data();
}

Oggle::String::Char* Oggle::String::Data()
{
    if (IsSmall()) return Rep.Stack;

    return Rep.Heap.Data();
}

Oggle::size_t Oggle::String::Length() const
{
    if (IsSmall()) return strlen(Rep.Stack);

    return Rep.Heap.Count();
}

Oggle::size_t Oggle::String::Capacity() const
{
    OGGLE_ENSURE(m_Capacity <= SmallStringCapacity, "String::Capacity() reported a capacity smaller than the SSO capacity. This doesn't inherently break anything but should be investigated.")
    return m_Capacity;
}

void Oggle::String::InitSmallString() {
    std::construct_at(&Rep.Stack);
    Rep.Stack[SmallStringCapacity] = '\0';
    m_Capacity = SmallStringCapacity;
}

void Oggle::String::DestroySmallString()
{
    std::destroy_at(&Rep.Stack);
}

void Oggle::String::InitBigString(size_t Capacity)
{
    auto Buffer = Allocate(Capacity);
    Buffer[Capacity] = '\0';
    std::construct_at(&Rep.Heap, Buffer, 0);
    m_Capacity = Capacity;
}

void Oggle::String::DestroyBigString()
{
    delete[] Rep.Heap.Data();
    std::destroy_at(&Rep.Heap);
}

Oggle::String::Char* Oggle::String::Allocate(size_t NumChars)
{
    const auto Buffer = new Char[NumChars + 1]; // Include space for a null terminator
    OGGLE_ASSERT(Buffer != nullptr);
    return Buffer;
}

void Oggle::String::Deallocate(Char* Buffer)
{
    delete[] Buffer;
}

bool Oggle::String::IsSmall() const
{
    return Capacity() <= SmallStringCapacity;
}

void Oggle::String::IncreaseCapacity()
{
    Reserve(2 * Capacity());
}

void Oggle::String::Reserve(size_t NumChars)
{
    if (NumChars <= Capacity())
    {
        return;
    }

    // Capacity() should never return anything less than SmallStringCapacity, so if we get here we for sure need to allocate
    Char* const NewBuffer = Allocate(NumChars);
    const size_t CachedLength = Length();

    if (IsSmall())
    {
        memcpy(NewBuffer, Rep.Stack, SmallStringCapacity);
        DestroySmallString();
        Rep.Heap = Span { NewBuffer, CachedLength };
    }
    else
    {
        const Char*  OldBuffer    = Rep.Heap.Data();
        memcpy(NewBuffer, Rep.Heap.Data(), CachedLength);
        delete[] OldBuffer;
        Rep.Heap = Span { NewBuffer, CachedLength };
    }

    m_Capacity = NumChars;
}

std::ostream& operator<<(std::ostream& Stream, const Oggle::String& String)
{
    return Stream << String.CStr();
}
