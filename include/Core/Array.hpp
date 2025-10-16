#pragma once
#include <array>
#include <memory>

template<typename T, std::size_t N>
using TArray = std::array<T, N>;

template<typename T, typename Allocator = std::allocator<T>>
using TDynamicArray = std::vector<T, Allocator>;