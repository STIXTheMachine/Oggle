#include <Core/Color.hpp>

namespace Detail
{
enum ColorHexValues : unsigned int
{
    White       = 0xFFFFFFFF,
    LightGray   = 0xDFDFDFFF,
    Gray        = 0xBABABAFF,
    DarkGray    = 0x878787FF,
    Black       = 0x000000FF,

    Red         = 0xFF0000FF,
    Green       = 0x00FF00FF,
    Blue        = 0x0000FFFF,

    Yellow      = 0xFFFF00FF,
    Magenta     = 0xFF00FFFF,
    Cyan        = 0x00FFFFFF,

    Emerald     = 0x2ECC71FF,
    Orange      = 0xF39C12FF,
    Purple      = 0xA907FFFF,
    Silver      = 0xBDC3C7FF,
    Turquoise   = 0x1ABC9CFF,

    Transparent = 0x00000000,
};
}



const ByteColor ByteColor::White       = FromHexRGBA(Detail::ColorHexValues::White);
const ByteColor ByteColor::LightGray   = FromHexRGBA(Detail::ColorHexValues::LightGray);
const ByteColor ByteColor::Gray        = FromHexRGBA(Detail::ColorHexValues::Gray);
const ByteColor ByteColor::DarkGray    = FromHexRGBA(Detail::ColorHexValues::DarkGray);
const ByteColor ByteColor::Black       = FromHexRGBA(Detail::ColorHexValues::Black);

const ByteColor ByteColor::Red         = FromHexRGBA(Detail::ColorHexValues::Red);
const ByteColor ByteColor::Green       = FromHexRGBA(Detail::ColorHexValues::Green);
const ByteColor ByteColor::Blue        = FromHexRGBA(Detail::ColorHexValues::Blue);

const ByteColor ByteColor::Yellow      = FromHexRGBA(Detail::ColorHexValues::Yellow);
const ByteColor ByteColor::Magenta     = FromHexRGBA(Detail::ColorHexValues::Magenta);
const ByteColor ByteColor::Cyan        = FromHexRGBA(Detail::ColorHexValues::Cyan);

const ByteColor ByteColor::Emerald     = FromHexRGBA(Detail::ColorHexValues::Emerald);
const ByteColor ByteColor::Orange      = FromHexRGBA(Detail::ColorHexValues::Orange);
const ByteColor ByteColor::Purple      = FromHexRGBA(Detail::ColorHexValues::Purple);
const ByteColor ByteColor::Silver      = FromHexRGBA(Detail::ColorHexValues::Silver);
const ByteColor ByteColor::Turquoise   = FromHexRGBA(Detail::ColorHexValues::Turquoise);

const ByteColor ByteColor::Transparent = FromHexRGBA(Detail::ColorHexValues::Transparent);



const FloatColor FloatColor::White       = FromHexRGBA(Detail::ColorHexValues::White);
const FloatColor FloatColor::LightGray   = FromHexRGBA(Detail::ColorHexValues::LightGray);
const FloatColor FloatColor::Gray        = FromHexRGBA(Detail::ColorHexValues::Gray);
const FloatColor FloatColor::DarkGray    = FromHexRGBA(Detail::ColorHexValues::DarkGray);
const FloatColor FloatColor::Black       = FromHexRGBA(Detail::ColorHexValues::Black);

const FloatColor FloatColor::Red         = FromHexRGBA(Detail::ColorHexValues::Red);
const FloatColor FloatColor::Green       = FromHexRGBA(Detail::ColorHexValues::Green);
const FloatColor FloatColor::Blue        = FromHexRGBA(Detail::ColorHexValues::Blue);

const FloatColor FloatColor::Yellow      = FromHexRGBA(Detail::ColorHexValues::Yellow);
const FloatColor FloatColor::Magenta     = FromHexRGBA(Detail::ColorHexValues::Magenta);
const FloatColor FloatColor::Cyan        = FromHexRGBA(Detail::ColorHexValues::Cyan);

const FloatColor FloatColor::Emerald     = FromHexRGBA(Detail::ColorHexValues::Emerald);
const FloatColor FloatColor::Orange      = FromHexRGBA(Detail::ColorHexValues::Orange);
const FloatColor FloatColor::Purple      = FromHexRGBA(Detail::ColorHexValues::Purple);
const FloatColor FloatColor::Silver      = FromHexRGBA(Detail::ColorHexValues::Silver);
const FloatColor FloatColor::Turquoise   = FromHexRGBA(Detail::ColorHexValues::Turquoise);

const FloatColor FloatColor::Transparent = FromHexRGBA(Detail::ColorHexValues::Transparent);

// =====================================================================================================================
// ===================================================== ByteColor =====================================================
// =====================================================================================================================
ByteColor::ByteColor() : R(0), G(0), B(0), A(0) {};
ByteColor::ByteColor(ComponentType R, ComponentType G, ComponentType B, ComponentType A)
    : R(R), G(G), B(B), A(A) {};
ByteColor::ByteColor(ComponentType R, ComponentType G, ComponentType B)
    : R(R), G(G), B(B), A(std::numeric_limits<ComponentType>::max()) {};

ByteColor::operator FloatColor() const
{
    static constexpr float ConversionFactor = 1.f / std::numeric_limits<ComponentType>::max();

    return FloatColor {
        static_cast<FloatColor::ComponentType>(R) * ConversionFactor,
        static_cast<FloatColor::ComponentType>(G) * ConversionFactor,
        static_cast<FloatColor::ComponentType>(B) * ConversionFactor,
        static_cast<FloatColor::ComponentType>(A) * ConversionFactor
    };
}

FloatColor ByteColor::ToFloatColor() const
{
    return FloatColor { *this };
}

ByteColor ByteColor::FromHexRGB(unsigned int HexRGB)
{
    return FromHexRGBA((HexRGB << 8) | 0xFF);
}

ByteColor ByteColor::FromHexRGBA(unsigned int HexRGBA)
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
FloatColor::FloatColor(ComponentType R, ComponentType G, ComponentType B, ComponentType A)
    : R(R), G(G), B(B), A(A) {};
FloatColor::FloatColor(ComponentType R, ComponentType G, ComponentType B)
    : R(R), G(G), B(B), A(1) {}

FloatColor FloatColor::FromHexRGB(unsigned int HexRGB)
{
    return FromHexRGBA((HexRGB << 8) | 0xFF);
}
FloatColor FloatColor::FromHexRGBA(unsigned int HexRGBA)
{
    return FloatColor { ByteColor::FromHexRGBA(HexRGBA) };
};

FloatColor::operator ByteColor() const
{
    constexpr auto ConversionFactor = std::numeric_limits<ByteColor::ComponentType>::max();
    return ByteColor {
        static_cast<ByteColor::ComponentType>(R * ConversionFactor),
        static_cast<ByteColor::ComponentType>(G * ConversionFactor),
        static_cast<ByteColor::ComponentType>(B * ConversionFactor),
        static_cast<ByteColor::ComponentType>(A * ConversionFactor)
    };
}

ByteColor FloatColor::ToByteColor() const
{
    return ByteColor { *this };
}
