//
// Created by jake on 1/2/26.
//
#include <Core/OggleType.hpp>
#include <Renderer/BitmapLoader.hpp>
#include <cstring>
#include <fstream>

DEFINE_LOG_CATEGORY(ImageLoad)

namespace fs = std::filesystem;
namespace Oggle {

template<typename T>
T ReadBytes(const std::byte* From)
{
    T Value;
    std::memcpy(&Value, From, sizeof(T));
    return Value;
}

std::vector<std::byte> BitmapLoader::Load(fs::path Path)
{
    std::string FullPath = fs::absolute(Path).lexically_normal();

    if (!std::filesystem::exists(Path))
    {
        LOG(ImageLoad, Error, FMT("Failed to load file {}: file does not exist.", FullPath));
        return {};
    }

    std::ifstream FileStream { Path, std::ios::binary | std::ios::in | std::ios::ate };

    if (FileStream.tellg() < 54)
    {
        LOG(ImageLoad, Error, FMT("Failed to load file {}: file is not a valid bitmap file.", FullPath));
        return {};
    }

    FileStream.seekg(0);

    std::byte Header[54];
    FileStream.read(reinterpret_cast<char*>(Header), sizeof(Header));

    if (Header[0] != std::byte{'B'} || Header[1] != std::byte{'M'})
    {
        LOG(ImageLoad, Error, FMT("Failed to load file {}: file is not a valid bitmap file.", FullPath));
        return {};
    }

    auto DataPos     = ReadBytes<uint32>(&Header[0x0A]);
    auto ImageWidth  = ReadBytes<uint32>(&Header[0x12]);
    auto ImageHeight = ReadBytes<uint32>(&Header[0x16]);
    auto ImageSize   = ReadBytes<uint32>(&Header[0x22]);

    if (DataPos == 0) { DataPos = 54; }
    if (ImageSize == 0) { ImageSize = ImageWidth * ImageHeight * 3; };

    std::vector<std::byte> Data(ImageSize);

    FileStream.read(reinterpret_cast<char*>(Data.data()), ImageSize);

    return Data;
}
} // Oggle