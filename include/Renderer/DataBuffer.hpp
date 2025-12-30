#pragma once
#include <span>
#include <vector>
#include <concepts>
#include <format>


constexpr std::byte operator""_b(const unsigned long long Val)
{
    return std::byte { static_cast<unsigned char>(Val) };
}

using BufferType = std::vector<std::byte>;
using BufferView = std::span<const std::byte>;

struct DataBuffer
{
    DataBuffer() = default;

    // Reserves size_t uninitialized bytes of buffer space.
    explicit DataBuffer(size_t InitialSize);

    // Reserves size_t bytes of buffer space and initializes all of them to InitialValue.
    DataBuffer(size_t, std::byte InitialValue);

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
    DataBuffer& operator<<(std::byte InData);

    size_t Size() const;
    size_t Capacity() const;
    void Reserve(size_t NewCapacity);
    void AddCapacity(size_t AdditionalCapacity);

    void Clear();

    auto begin() const { return Data.begin(); };
    auto end() const { return Data.end(); };

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
        auto Out = Ctx.out();

        std::format_to(Out, "{}", "============== Begin Buffer Object ==============\n[");

        for (size_t NumBytesWritten = 0; const auto Byte : Buffer)
        {
            const auto ByteInt = std::to_integer<unsigned int>(Byte);

            std::format_to(Out, "{:02X}", ByteInt);
            ++NumBytesWritten;

            if (NumBytesWritten < Buffer.Size())
            {
                if (NumBytesWritten % 16 == 0)
                {
                    std::format_to(Out, "{}", "]\n[");
                }
                else
                {
                    std::format_to(Out, "{}", " ");
                }
            }
        }

        std::format_to(Out, "{}", "]\n=============== End Buffer Object ===============");

        return Out;
    }
};
