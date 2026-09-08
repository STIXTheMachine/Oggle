#include "Container/String.hpp"
#include <cstring>

Oggle::String::String()
{
    InitSmallStringBuffer();
    m_Capacity = SmallStringCapacity;
}

Oggle::String::String(size_t NumChars)
{
    if (NumChars <= SmallStringCapacity)
    {
        InitSmallStringBuffer();
        m_Capacity = SmallStringCapacity;
    }
    else
    {
        InitHeapBuffer(NumChars);
        m_Capacity = NumChars;
    }
}

Oggle::String::String(size_t NumChars, Char Fill)
    : String(NumChars)
{
    if (IsSmall())
    {
        memset(Rep.Stack, Fill, SmallStringCapacity);
    }
    else
    {
        memset(Rep.Heap.Data(), Fill, NumChars);
    }
}

Oggle::String::String(const char* String)
{
    const size_t Length = strlen(String);

    if (Length <= SmallStringCapacity)
    {
        InitSmallStringBuffer();
        m_Capacity = SmallStringCapacity;
        strlcpy(Rep.Stack, String, SmallStringBufSize);
    }
    else
    {
        InitHeapBuffer(Length);
        m_Capacity = Length;
        strlcpy(Rep.Heap.Data(), String, Length + 1);
    }
}

Oggle::String::String(const String& Other)
    : String(Other.CStr())
{

}

Oggle::String::String(String&& Other)
{
    if (Other.IsSmall())
    {
        InitSmallStringBuffer();
        memcpy(&Rep.Stack, &Other.Rep.Stack, sizeof(Rep.Stack));
        m_Capacity = SmallStringCapacity;
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
    OGGLE_UNIMPLEMENTED();
    return *this;
}

Oggle::String& Oggle::String::operator=(String&& Other)
{
    OGGLE_UNIMPLEMENTED();
    return *this;
}

Oggle::String& Oggle::String::operator+(const String& Rhs)
{
    OGGLE_UNIMPLEMENTED();
    return *this += Rhs;
}

Oggle::String& Oggle::String::operator+=(const Char* String)
{
    OGGLE_UNIMPLEMENTED();
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

Oggle::size_t Oggle::String::Size() const
{
    if (IsSmall()) return strlen(Rep.Stack);

    return Rep.Heap.Count();
}

Oggle::size_t Oggle::String::Capacity() const
{
    OGGLE_ENSURE(m_Capacity <= SmallStringCapacity, "String::Capacity() reported a capacity smaller than the SSO capacity. This doesn't inherently break anything but should be investigated.")
    return m_Capacity;
}

void Oggle::String::InitSmallStringBuffer() {
    std::construct_at(&Rep.Stack);
    Rep.Stack[SmallStringCapacity] = '\0';
}

void Oggle::String::InitHeapBuffer(size_t NumChars)
{
    auto Buffer = Allocate(NumChars);
    Buffer[NumChars] = '\0';
    std::construct_at(&Rep.Heap, Buffer, NumChars);
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

    Char* const NewBuffer = Allocate(NumChars);

    if (IsSmall())
    {
        const size_t Length = strlen(Rep.Stack);
        OGGLE_ENSURE(Length <= SmallStringCapacity);
        memcpy(NewBuffer, Rep.Stack, SmallStringCapacity);

        std::destroy_at(&Rep.Stack); // *Technically* not necessary but something something UB
        std::construct_at(&Rep.Heap, NewBuffer, Length);
    }
    else
    {
        const size_t Length = Rep.Heap.Count();
        const Char* OldBuffer = Rep.Heap.Data();

        memcpy(NewBuffer, OldBuffer, Length);
        delete[] OldBuffer;

        Rep.Heap = Span { NewBuffer, Length };
    }

    m_Capacity = NumChars;
}
