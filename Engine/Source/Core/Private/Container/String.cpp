#include "Container/String.hpp"
#include <cstring>

Oggle::String::String()
{
    std::construct_at(&Rep.Stack);
    m_Capacity = SmallStringCapacity;
}

Oggle::String::String(size_t NumChars)
{
    if (NumChars <= SmallStringCapacity)
    {
        std::construct_at(&Rep.Stack);
        m_Capacity = SmallStringCapacity;
    }
    else
    {
        auto Buffer = Allocate(NumChars);
        std::construct_at(&Rep.Heap, Buffer, NumChars);
    }
}

Oggle::String::String(size_t NumChars, Char Fill) : String(NumChars)
{
    if (IsSmall())
    {
        memset(Rep.Stack, Fill, SmallStringCapacity);
        Rep.Stack[SmallStringCapacity] = '\0';
    }
    else
    {

    }
}

Oggle::String::String(const char* String)
{
    OGGLE_UNIMPLEMENTED();
}

Oggle::String::String(const String& Other)
{
    OGGLE_UNIMPLEMENTED();
}

Oggle::String::String(String&& Other)
{
    OGGLE_UNIMPLEMENTED();
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

Oggle::String::Char* Oggle::String::Allocate(size_t NumChars)
{
    const auto Buffer = new Char[NumChars + 1]; // Include space for a null terminator
    Buffer[NumChars] = '\0';
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
