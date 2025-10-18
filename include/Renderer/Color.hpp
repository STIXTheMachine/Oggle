#pragma once
#include <Renderer/OpenGL.hpp>
#include <limits>

using ColorComponent = float;
static_assert(std::is_arithmetic_v<ColorComponent>, "ColorComponent must be an arithmetic type.");

static constexpr double ColorComponentNormalizationFactor   = 1.0 / std::numeric_limits<ColorComponent>::max();
static constexpr double ColorComponentDenormalizationFactor =       std::numeric_limits<ColorComponent>::max();

struct Color
{
    Color() = default;
    ColorComponent R, G, B, A;
};