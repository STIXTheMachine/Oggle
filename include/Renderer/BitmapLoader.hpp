#pragma once
#include <Core/Logging/Logging.hpp>
#include <filesystem>

DECLARE_LOG_CATEGORY(ImageLoad, Info, Default);

namespace Oggle
{
struct BitmapLoader
{
    std::vector<std::byte> Load(std::filesystem::path Path);
};
} // Oggle