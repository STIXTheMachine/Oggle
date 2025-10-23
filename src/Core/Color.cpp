#include <Core/Color.hpp>

namespace Detail
{
enum ColorHexValues
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



constinit const ByteColor ByteColor::White       = FromHexRGBA(Detail::ColorHexValues::White);
constinit const ByteColor ByteColor::LightGray   = FromHexRGBA(Detail::ColorHexValues::LightGray);
constinit const ByteColor ByteColor::Gray        = FromHexRGBA(Detail::ColorHexValues::Gray);
constinit const ByteColor ByteColor::DarkGray    = FromHexRGBA(Detail::ColorHexValues::DarkGray);
constinit const ByteColor ByteColor::Black       = FromHexRGBA(Detail::ColorHexValues::Black);

constinit const ByteColor ByteColor::Red         = FromHexRGBA(Detail::ColorHexValues::Red);
constinit const ByteColor ByteColor::Green       = FromHexRGBA(Detail::ColorHexValues::Green);
constinit const ByteColor ByteColor::Blue        = FromHexRGBA(Detail::ColorHexValues::Blue);

constinit const ByteColor ByteColor::Yellow      = FromHexRGBA(Detail::ColorHexValues::Yellow);
constinit const ByteColor ByteColor::Magenta     = FromHexRGBA(Detail::ColorHexValues::Magenta);
constinit const ByteColor ByteColor::Cyan        = FromHexRGBA(Detail::ColorHexValues::Cyan);

constinit const ByteColor ByteColor::Emerald     = FromHexRGBA(Detail::ColorHexValues::Emerald);
constinit const ByteColor ByteColor::Orange      = FromHexRGBA(Detail::ColorHexValues::Orange);
constinit const ByteColor ByteColor::Purple      = FromHexRGBA(Detail::ColorHexValues::Purple);
constinit const ByteColor ByteColor::Silver      = FromHexRGBA(Detail::ColorHexValues::Silver);
constinit const ByteColor ByteColor::Turquoise   = FromHexRGBA(Detail::ColorHexValues::Turquoise);

constinit const ByteColor ByteColor::Transparent = FromHexRGBA(Detail::ColorHexValues::Transparent);



constinit const FloatColor FloatColor::White       = FromHexRGBA(Detail::ColorHexValues::White);
constinit const FloatColor FloatColor::LightGray   = FromHexRGBA(Detail::ColorHexValues::LightGray);
constinit const FloatColor FloatColor::Gray        = FromHexRGBA(Detail::ColorHexValues::Gray);
constinit const FloatColor FloatColor::DarkGray    = FromHexRGBA(Detail::ColorHexValues::DarkGray);
constinit const FloatColor FloatColor::Black       = FromHexRGBA(Detail::ColorHexValues::Black);

constinit const FloatColor FloatColor::Red         = FromHexRGBA(Detail::ColorHexValues::Red);
constinit const FloatColor FloatColor::Green       = FromHexRGBA(Detail::ColorHexValues::Green);
constinit const FloatColor FloatColor::Blue        = FromHexRGBA(Detail::ColorHexValues::Blue);

constinit const FloatColor FloatColor::Yellow      = FromHexRGBA(Detail::ColorHexValues::Yellow);
constinit const FloatColor FloatColor::Magenta     = FromHexRGBA(Detail::ColorHexValues::Magenta);
constinit const FloatColor FloatColor::Cyan        = FromHexRGBA(Detail::ColorHexValues::Cyan);

constinit const FloatColor FloatColor::Emerald     = FromHexRGBA(Detail::ColorHexValues::Emerald);
constinit const FloatColor FloatColor::Orange      = FromHexRGBA(Detail::ColorHexValues::Orange);
constinit const FloatColor FloatColor::Purple      = FromHexRGBA(Detail::ColorHexValues::Purple);
constinit const FloatColor FloatColor::Silver      = FromHexRGBA(Detail::ColorHexValues::Silver);
constinit const FloatColor FloatColor::Turquoise   = FromHexRGBA(Detail::ColorHexValues::Turquoise);

constinit const FloatColor FloatColor::Transparent = FromHexRGBA(Detail::ColorHexValues::Transparent);