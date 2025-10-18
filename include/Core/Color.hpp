#pragma once
#include <Renderer/OpenGL.hpp>
#include <limits>

struct ByteColor;
struct FloatColor;

/// Stores color information in 4 bytes for R, G, B, A.
struct ByteColor
{
    using ComponentType = GLubyte;
    ComponentType R, G, B, A;

    constexpr ByteColor() = default;
    constexpr ByteColor(ComponentType R, ComponentType G, ComponentType B, ComponentType A);
    constexpr ByteColor(ComponentType R, ComponentType G, ComponentType B);

    explicit constexpr operator FloatColor() const;

    static constexpr ByteColor FromHexRGB(unsigned int HexRGB);
    static constexpr ByteColor FromHexRGBA(unsigned int HexRGBA);

    static_assert(std::is_arithmetic_v<ComponentType>, "ColorComponent must be an arithmetic type.");
};

/// Stores color information in 4 floats for R, G, B, A. Values are assumed to be in [0, 1].
struct FloatColor
{
    using ComponentType = GLfloat;
    ComponentType R, G, B, A;

    constexpr FloatColor() = default;
    constexpr FloatColor(ComponentType R, ComponentType G, ComponentType B, ComponentType A);
    constexpr FloatColor(ComponentType R, ComponentType G, ComponentType B);

    static constexpr FloatColor FromHexRGB(unsigned int HexRGB);
    static constexpr FloatColor FromHexRGBA(unsigned int HexRGBA);

    explicit constexpr operator ByteColor() const;

    static_assert(std::is_arithmetic_v<ComponentType>, "ColorComponent must be an arithmetic type.");
};

// =====================================================================================================================
// ======================================== Constexpr stuff implemenation below ========================================
// =====================================================================================================================

// =====================================================================================================================
// ===================================================== ByteColor =====================================================
// =====================================================================================================================
constexpr ByteColor::ByteColor(ComponentType R, ComponentType G, ComponentType B, ComponentType A)
    : R(R), G(G), B(B), A(A) {};
constexpr ByteColor::ByteColor(ComponentType R, ComponentType G, ComponentType B)
    : R(R), G(G), B(B), A(1) {};

constexpr ByteColor::operator FloatColor() const
{
    static constexpr float ConversionFactor = 1.f / std::numeric_limits<ComponentType>::max();

    return FloatColor {
        static_cast<FloatColor::ComponentType>(R) * ConversionFactor,
        static_cast<FloatColor::ComponentType>(G) * ConversionFactor,
        static_cast<FloatColor::ComponentType>(B) * ConversionFactor,
        static_cast<FloatColor::ComponentType>(A) * ConversionFactor
    };
}

constexpr ByteColor ByteColor::FromHexRGB(unsigned int HexRGB)
{
    return FromHexRGBA(HexRGB << 8 | 0xFF);
}

constexpr ByteColor ByteColor::FromHexRGBA(unsigned int HexRGBA)
{
    const ComponentType R = (HexRGBA >> 24) & 0xFF;
    const ComponentType G = (HexRGBA >> 16) & 0xFF;
    const ComponentType B = (HexRGBA >> 8)  & 0xFF;
    const ComponentType A = (HexRGBA >> 0)  & 0xFF;

    return ByteColor { R, G, B, A };
}

// =====================================================================================================================
// ==================================================== FloatColor =====================================================
// =====================================================================================================================
constexpr FloatColor::FloatColor(ComponentType R, ComponentType G, ComponentType B, ComponentType A)
    : R(R), G(G), B(B), A(A) {};
constexpr FloatColor::FloatColor(ComponentType R, ComponentType G, ComponentType B)
    : R(R), G(G), B(B), A(1) {}

constexpr FloatColor FloatColor::FromHexRGB(unsigned int HexRGB)
{
    return FromHexRGBA(HexRGB << 8 | 0xFF);
}
constexpr FloatColor FloatColor::FromHexRGBA(unsigned int HexRGBA)
{
    return FloatColor { ByteColor::FromHexRGBA(HexRGBA) };
};

constexpr FloatColor::operator ByteColor() const
{
    static constexpr float ConversionFactor = 255.f;
    return ByteColor {
        static_cast<ByteColor::ComponentType>(R * ConversionFactor),
        static_cast<ByteColor::ComponentType>(G * ConversionFactor),
        static_cast<ByteColor::ComponentType>(B * ConversionFactor),
        static_cast<ByteColor::ComponentType>(A * ConversionFactor)
    };
}
