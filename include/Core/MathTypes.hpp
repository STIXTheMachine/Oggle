#pragma once
#include "Array.hpp"

template<typename ScalarType, std::size_t Dimension>
requires std::is_arithmetic_v<ScalarType>
using TVector = TArray<ScalarType, Dimension>;

using Vec2 = TVector<float, 2>;
using Vec3 = TVector<float, 3>;
using Vec4 = TVector<float, 4>;