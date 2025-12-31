#pragma once
#include <Core/OggleType.hpp>
#include <Core/Array.hpp>
#include <span>
#include <vector>
#include <concepts>
#include <format>
#include <ranges>


constexpr byte operator""_b(const unsigned long long Val)
{
    return byte { static_cast<unsigned char>(Val) };
}

using BufferType = TDynamicArray<byte>;
using BufferView = std::span<const byte>;

struct DataBuffer
{
    DataBuffer() = default;

    // Reserves size_t uninitialized bytes of buffer space.
    explicit DataBuffer(size_t InitialSize);

    // Reserves size_t bytes of buffer space and initializes all of them to InitialValue.
    DataBuffer(size_t, byte InitialValue);

    DataBuffer& operator=(const DataBuffer& Other) = default;
    DataBuffer(const DataBuffer& Other) = default;

    DataBuffer& operator=(DataBuffer&& Other) noexcept;
    DataBuffer(DataBuffer&& Other)  noexcept;

    ~DataBuffer() = default;

    // Takes ownership of the input buffer.
    // Moves from the input. Does not copy the data.
    template<typename T>
    requires std::constructible_from<BufferType, T&&>
    void AcquireData(T&& NewData) { Data = std::move(NewData); }

    // Copies the input buffer. Leaves the input buffer unaffected.
    void CopyData(const BufferType& NewData);

    // Returns a view of the data currently held by the buffer.
    BufferView GetData() const { return  Data; }

    // Stream bytes into buffer.
    // Pair with Reserve() or AddCapacity() to avoid unnecessary reallocations
    DataBuffer& operator<<(byte InData);
    DataBuffer& operator<<(BufferView InData);

    size_t Size() const;
    size_t Capacity() const;
    void Reserve(size_t NewCapacity);
    void AddCapacity(size_t AdditionalCapacity);

    void Clear();

private:
    BufferType Data;
};

template<>
struct std::formatter<DataBuffer>
{
    constexpr auto parse(std::format_parse_context& Ctx)
    {
        auto it = Ctx.begin();

        while (it != Ctx.end() && *it != '}') {
            ++it;
        }

        return it;
    }

    auto format(const DataBuffer& Buffer, std::format_context& Ctx) const
    {
        using namespace std::views;
        auto Out = Ctx.out();

        std::string_view Header { "============== Begin Buffer Object ==============\n[" };
        std::string_view Footer { "]\n=============== End Buffer Object ===============" };

        std::format_to(Out, "{}", Header);

        for (const auto [Index, Byte] : Buffer.GetData() | enumerate)
        {
           const auto ByteAsInt = std::to_integer<unsigned int>(Byte);
           std::format_to(Out, "{:02X}", ByteAsInt);

           if ((Index + 1) < Buffer.Size())
           {
               if ((Index + 1) % 16 == 0)
               {
                   std::format_to(Out, "{}", "]\n[");
               }
               else
               {
                   std::format_to(Out, "{}", " ");
               }
           }
        }

        std::format_to(Out, "{}", Footer);

        return Out;
    }
};
