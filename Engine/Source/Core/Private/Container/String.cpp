#include "Container/String.hpp"
#include <cstring>

namespace Oggle
{
    namespace Detail
    {
        static size_t Strncpy_Safe(Char* Dest, const Char* Source, const size_t DestSize)
        {
            if (DestSize == 0) return 0;

            size_t CharsCopied = 0;
            while (CharsCopied < DestSize - 1)
            {
                if ((*Dest++ = *Source++) == '\0') return CharsCopied;
                CharsCopied++;
            }

            size_t RemainingBufferLength = DestSize - CharsCopied;
            while (RemainingBufferLength > 0)
            {
                *Dest++ = '\0';
                RemainingBufferLength--;
            }

            return CharsCopied;
        }

        HeapString::HeapString(size_t Capacity)
        {
            Buffer = Allocate(Capacity);
            Length = 0;
        }

        HeapString::HeapString(const Char* String)
        {
            Length = strlen(String);
            Buffer = Allocate(Length);
            Strncpy_Safe(Buffer, String, Length);
        }

        HeapString::HeapString(const StackString& Other) : HeapString(Other.Buffer)
        {
        }

        HeapString::HeapString(const HeapString& Other)
        {
            Length = Other.Length;
            Buffer = Allocate(Length);
            Strncpy_Safe(Buffer, Other.Buffer, Length);
        }

        HeapString::HeapString(HeapString&& Other)
        {
            Length = Other.Length;
            Buffer = Other.Buffer;
            Other = {};
        }

        HeapString& HeapString::operator=(const HeapString& Other)
        {
            if (this == &Other) return *this;

            Length = Other.Length;
            Buffer = Allocate(Length);
            Strncpy_Safe(Buffer, Other.Buffer, Length);

            return *this;
        }

        HeapString& HeapString::operator=(HeapString&& Other)
        {
            if (this == &Other) return *this;

            Length = Other.Length;
            Buffer = Other.Buffer;
            Other = {};

            return *this;
        }

        HeapString::~HeapString()
        {
            Deallocate(Buffer);
            Length = 0;
        }

        Char* HeapString::Allocate(size_t Capacity)
        {
            Char* Buffer = new Char[Capacity + 1];
            OGGLE_ASSERT(Buffer);
            return Buffer;
        }

        void HeapString::Deallocate(Char* Data)
        {
            delete [] Data;
        }

        Char* HeapString::Reallocate(size_t NewCapacity)
        {
            Char* NewBuffer = new Char[NewCapacity + 1];

            Strncpy_Safe(NewBuffer, Buffer, NewCapacity);
            delete [] Buffer;

            Buffer = NewBuffer;
            Length = NewCapacity;

            return NewBuffer;
        }

        StackString::StackString(const Char* String)
        {
            const size_t Length = strlen(String);
            OGGLE_ENSURE(Length < SmallStringBufferSize);
            Strncpy_Safe(Buffer, String, SmallStringBufferSize);
        }

        size_t StackString::Length() const
        {
            return strlen(Buffer);
        }
    }


    std::ostream& operator<<(std::ostream& Stream, const Oggle::String& String)
    {
        return Stream << String.CStr();
    }

    String::String()
    {
        Rep.Stack = {};
        m_Capacity = Detail::SmallStringCapacity;
    }

    String::String(size_t Capacity)
    {
        if (Capacity <= Detail::SmallStringCapacity)
        {
            Rep.Stack = {};
            m_Capacity = Detail::SmallStringCapacity;
        }
        else
        {
            Rep.Heap = { Capacity };
            m_Capacity = Capacity;
        }
    }

    String::String(size_t NumChars, Char Fill)
    {
        if (NumChars <= Detail::SmallStringCapacity)
        {
            Rep.Stack = {};
            m_Capacity = Detail::SmallStringCapacity;
            memset(Rep.Stack.Buffer, Fill, NumChars); // Detail::StackString fills its buffer with \0 on construction, we don't need to manually add a null terminator
        }
        else
        {
            Rep.Heap = { NumChars };
            Rep.Heap.Length = NumChars;
            m_Capacity = NumChars;
            memset(Rep.Heap.Buffer, Fill, NumChars);
            Rep.Heap.Buffer[NumChars] = '\0'; // As an optimization, Detail::HeapString does NOT fill its buffer with \0, so we DO have to do it here.
        }
    }

    String::String(const char* String)
    {
        const size_t Length = strlen(String);
        if (Length <= Detail::SmallStringCapacity)
        {
            Rep.Stack = Detail::StackString { String };
        }
        else
        {
            Rep.Heap = Detail::HeapString { String };
        }
    }

    String::String(const String& Other)
    {
        if (Other.IsSmall())
        {
            Rep.Stack = Other.Rep.Stack;
        }
        else
        {
            Rep.Heap = Other.Rep.Heap;
        }
    }

    String::String(String&& Other)
    {
        if (Other.IsSmall())
        {
            Rep.Stack = Other.Rep.Stack;
        }
        else
        {
            Rep.Heap = std::move(Other.Rep.Heap);
        }
    }

    String::String(StringView View)
    {
        OGGLE_UNIMPLEMENTED()
    }

    String& String::operator=(const String& Other)
    {
        if (this == &Other) return *this;

        if (Other.IsSmall())
        {
            Rep.Stack = Other.Rep.Stack;
        }
        else
        {
            Rep.Heap = Other.Rep.Heap;
        }

        return *this;
    }

    String& String::operator=(String&& Other)
    {
        if (this == &Other) return *this;

        if (Other.IsSmall())
        {
            Rep.Stack = Other.Rep.Stack;
        }
        else
        {
            Rep.Heap = std::move(Other.Rep.Heap);
        }

        return *this;
    }

    String& String::operator+=(const Char* Rhs)
    {
        OGGLE_UNIMPLEMENTED()
        return *this;
    }

    String& String::operator+=(const String& Rhs)
    {
        OGGLE_UNIMPLEMENTED()
        return *this;
    }

    String& String::operator+=(StringView Rhs)
    {
        OGGLE_UNIMPLEMENTED()
        return *this;
    }

    String operator+(String Lhs, const String& Rhs)
    {
        return Lhs += Rhs;
    }

    String operator+(String Lhs, const Char* Rhs)
    {
        return Lhs += Rhs;
    }

    String operator+(String Lhs, StringView Rhs)
    {
        return Lhs += Rhs;
    }

    String& String::operator=(StringView View)
    {
        OGGLE_UNIMPLEMENTED()
        return *this;
    }

    void String::Reserve(size_t NumChars)
    {
        OGGLE_UNIMPLEMENTED()
    }

    const Char* String::CStr() const
    {
        if (IsSmall()) return Rep.Stack.Buffer;

        return Rep.Heap.Buffer;
    }

    bool String::operator==(const String& Other) const
    {
        OGGLE_UNIMPLEMENTED()
        return false;
    }

    bool String::operator==(const Char* Other) const
    {
        OGGLE_UNIMPLEMENTED()
        return false;
    }

    bool String::operator==(StringView Other) const
    {
        OGGLE_UNIMPLEMENTED()
        return false;
    }

    Char* String::Data()
    {
        OGGLE_UNIMPLEMENTED()
        return nullptr;
    }

    size_t String::Length() const
    {
        if (IsSmall()) return Rep.Stack.Length();

        return Rep.Heap.Length;
    }

    size_t String::Capacity() const
    {
        OGGLE_ENSURE(m_Capacity >= Detail::SmallStringCapacity)
        return m_Capacity;
    }

    bool String::IsEmpty() const
    {
        return Length() == 0;
    }

    bool String::IsSmall() const
    {
        return m_Capacity <= Detail::SmallStringCapacity;
    }

    void String::IncreaseCapacity()
    {
        OGGLE_UNIMPLEMENTED()
    }
} // namespace Oggle
