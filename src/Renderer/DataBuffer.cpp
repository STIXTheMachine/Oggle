#include "Renderer/DataBuffer.hpp"
#include <utility>

DataBuffer::DataBuffer(size_t InitialSize)
    : Data(InitialSize)
{}

DataBuffer::DataBuffer(size_t InitialSize, std::byte InitialValue)
    : Data(InitialSize, InitialValue)
{}

DataBuffer& DataBuffer::operator=(DataBuffer&& Other) noexcept
{
    Data = std::exchange(Other.Data, {});
    return *this;
}

DataBuffer::DataBuffer(DataBuffer&& Other) noexcept
    : Data(std::exchange(Other.Data, {}))
{}

void DataBuffer::CopyData(const BufferType& NewData)
{
    Data = NewData;
}

DataBuffer& DataBuffer::operator<<(const std::byte InData)
{
    Data.push_back(InData);
    return *this;
}

size_t DataBuffer::Size() const
{
    return Data.size();
}

size_t DataBuffer::Capacity() const
{
    return Data.capacity();
}

void DataBuffer::Reserve(size_t NewCapacity)
{
    Data.reserve(NewCapacity);
}

void DataBuffer::AddCapacity(size_t AdditionalCapacity)
{
    Data.reserve(Data.capacity() + AdditionalCapacity);
}

void DataBuffer::Clear()
{
    Data.clear();
}
