#include "Core/Container/String.hpp"
#include <cstring>

namespace Oggle
{
String::String()
{
    std::construct_at(&Rep.Stack);
    Capacity = SmallStringCapacity;
}

String::String(size_t NumChars)
{
    if (NumChars <= SmallStringCapacity)
    {
        std::construct_at(&Rep.Stack);
        Capacity = SmallStringCapacity;
    }
    else
    {
        std::construct_at(&Rep.Heap, Allocate(NumChars), 0);
        Capacity = NumChars;
    }
}

String::String(size_t NumChars, Char Fill) : String(NumChars)
{
    if (NumChars <= SmallStringCapacity)
    {
        std::construct_at(&Rep.Stack);
        memset(Rep.Stack.StackBuffer, Fill, NumChars);
        Capacity = SmallStringCapacity;
    }
    else
    {
        std::construct_at(&Rep.Heap, Allocate(NumChars), NumChars);
        memset(Rep.Heap.HeapBuffer, Fill, NumChars);
        Capacity = NumChars;
    }
}

String::String(const char* String)
{
    if (const size_t Length = strlen(String); Length <= SmallStringCapacity)
    {
        std::construct_at(&Rep.Stack);
        strncpy(Rep.Stack.StackBuffer, String, Length);
        Capacity = SmallStringCapacity;
    }
    else
    {
        std::construct_at(&Rep.Heap, Allocate(Length), Length);
        strncpy(Rep.Heap.HeapBuffer, String, Length);
        Capacity = Length;
    }
}

String::String(const String& Other)
{
    if (Other.IsSmallString())
    {
        std::construct_at(&Rep.Stack, Other.Rep.Stack);
        Capacity = SmallStringCapacity;
    }
    else
    {
        std::construct_at(&Rep.Heap, Allocate(Other.Rep.Heap.Length), Other.Rep.Heap.Length);
        strncpy(Rep.Heap.HeapBuffer, Other.Rep.Heap.HeapBuffer, Other.Rep.Heap.Length);
        Capacity = Other.Capacity;
    }
}

String::String(String&& Other)
{
    if (Other.IsSmallString())
    {
        std::construct_at(&Rep.Stack, Other.Rep.Stack);
        Capacity = SmallStringCapacity;
    }
    else
    {
        std::construct_at(&Rep.Heap, Other.Rep.Heap);
        Capacity = Other.Capacity;
        Other    = String {};
    }
}

String& String::operator=(const String& Other)
{
    if (this == &Other) return *this;

    if (IsSmallString())
    {
        std::destroy_at(&Rep.Stack);
    }
    else
    {
        Deallocate(Rep.Heap.HeapBuffer);
        std::destroy_at(&Rep.Heap);
    }

    if (Other.IsSmallString())
    {
        std::construct_at(&Rep.Stack, Other.Rep.Stack);
    }
    else
    {
        std::construct_at(&Rep.Heap, Allocate(Other.Rep.Heap.Length), Other.Rep.Heap.Length);
        strncpy(Rep.Heap.HeapBuffer, Other.Rep.Heap.HeapBuffer, Other.Rep.Heap.Length);
        Capacity = Other.Capacity;
    }

    return *this;
}

String& String::operator=(String&& Other)
{
    if (this == &Other) return *this;

    if (IsSmallString())
    {
        std::destroy_at(&Rep.Stack);
    }
    else
    {
        Deallocate(Rep.Heap.HeapBuffer);
        std::destroy_at(&Rep.Heap);
    }

    if (Other.IsSmallString())
    {
        std::construct_at(&Rep.Stack, Other.Rep.Stack);
    }
    else
    {
        std::construct_at(&Rep.Heap, Other.Rep.Heap);
        Capacity = Other.Capacity;
        Other    = String {};
    }

    return *this;
}

const char* String::CStr() const
{
    if (IsSmallString()) return Rep.Stack.StackBuffer;

    return Rep.Heap.HeapBuffer;
}

size_t String::GetLength() const
{
    if (IsSmallString())
    {
        return strlen(Rep.Stack.StackBuffer);
    }

    return Rep.Heap.Length;
}

size_t String::GetCapacity() const
{
    OGGLE_ENSURE_MSG(Capacity >= SmallStringCapacity, "String::GetCapacity() reported a Capacity of less than SmallStringCapacity. This doesn't inherently break anything but it's weird that it happened.")
    return Capacity;
}

String::Char* String::Allocate(size_t NumChars)
{
    Char* Buffer = new Char[NumChars + 1];
    OGGLE_ASSERT_MSG(Buffer, "Failed to allocate String buffer");
    return Buffer;
}

void String::Deallocate(Char* Buffer)
{
    delete [] Buffer;
}

bool String::IsSmallString() const
{
    return Capacity <= SmallStringCapacity;
}

void String::DoubleCapacity()
{
    // Create new buffer
    const size_t NewCapacity = 2 * Capacity;
    Char*        NewBuffer   = Allocate(NewCapacity);

    strncpy(NewBuffer, Rep.Heap.HeapBuffer, Rep.Heap.Length);

    // Free old buffer
    delete[] Rep.Heap.HeapBuffer;

    Rep.Heap.HeapBuffer = NewBuffer;
    Capacity        = NewCapacity;
}

std::ostream& operator<<(std::ostream& Stream, const String& String)
{
    return Stream << String.CStr();
}
}
