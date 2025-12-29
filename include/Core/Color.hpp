#pragma once
#include <Renderer/OpenGL.hpp>
#include <limits>

struct ByteColor;
struct FloatColor;

/// Stores color information in 4 bytes for R, G, B, A. Byte order is 0xRRGGBBAA.
struct ByteColor
{
    using ComponentType = GLubyte;
    ComponentType R, G, B, A;

    ByteColor();
    ByteColor(ComponentType R, ComponentType G, ComponentType B, ComponentType A);
    ByteColor(ComponentType R, ComponentType G, ComponentType B);

    explicit operator FloatColor() const;

    static ByteColor FromHexRGB(unsigned int HexRGB);
    static ByteColor FromHexRGBA(unsigned int HexRGBA);

    static const ByteColor White ;
    static const ByteColor LightGray;
    static const ByteColor Gray;
    static const ByteColor DarkGray;
    static const ByteColor Black;

    static const ByteColor Red;
    static const ByteColor Green;
    static const ByteColor Blue;

    static const ByteColor Yellow;
    static const ByteColor Magenta;
    static const ByteColor Cyan;

    static const ByteColor Emerald;
    static const ByteColor Orange;
    static const ByteColor Purple;
    static const ByteColor Silver;
    static const ByteColor Turquoise;

    static const ByteColor Transparent;

    static_assert(std::is_arithmetic_v<ComponentType>, "ColorComponent must be an arithmetic type.");
};

/// Stores color information in 4 floats for R, G, B, A. Values are assumed to be in [0, 1].
struct FloatColor
{
    using ComponentType = GLfloat;
    ComponentType R, G, B, A;

    FloatColor() = default;
    FloatColor(ComponentType R, ComponentType G, ComponentType B, ComponentType A);
    FloatColor(ComponentType R, ComponentType G, ComponentType B);

    static FloatColor FromHexRGB(unsigned int HexRGB);
    static FloatColor FromHexRGBA(unsigned int HexRGBA);

    explicit operator ByteColor() const;

    static const FloatColor White;
    static const FloatColor LightGray;
    static const FloatColor Gray;
    static const FloatColor DarkGray;
    static const FloatColor Black;

    static const FloatColor Red;
    static const FloatColor Green;
    static const FloatColor Blue;

    static const FloatColor Yellow;
    static const FloatColor Magenta;
    static const FloatColor Cyan;

    static const FloatColor Emerald;
    static const FloatColor Orange;
    static const FloatColor Purple;
    static const FloatColor Silver;
    static const FloatColor Turquoise;

    static const FloatColor Transparent;

    static_assert(std::is_arithmetic_v<ComponentType>, "ColorComponent must be an arithmetic type.");
};